#include "parser.h"

namespace sfsm {
const char Parser::OR_OP = '|';
const char Parser::STAR_OP = '*';
const char Parser::VIRTUAL_CAT_OP = '1'; // virtual operation
const char Parser::LEFT_BRACKET = '(';
const char Parser::RIGHT_BRACKET = ')';
const char Parser::RANGE_START = '[';
const char Parser::RANGE_MID = '-';
const char Parser::RANGE_END = ']';

unordered_map<char, int> Parser::OP_PRIORITY_MAP{{'|', 1}, {'*', 4}, {'1', 1}};

void Parser::runUnionOp(vector<ThompsonNFA> &valueStack) {
  // TODO Exception
  auto c1 = valueStack.back();
  valueStack.pop_back();
  auto c2 = valueStack.back();
  valueStack.pop_back();
  valueStack.push_back(this->tc.unionExpression(c2, c1));
}

void Parser::runConcatOp(vector<ThompsonNFA> &valueStack) {
  // TODO Exception
  auto c1 = valueStack.back();
  valueStack.pop_back();
  auto c2 = valueStack.back();
  valueStack.pop_back();
  valueStack.push_back(tc.concatExpression(c2, c1));
}

void Parser::runStarOp(vector<ThompsonNFA> &valueStack) {
  // TODO Exception
  auto c = valueStack.back();
  valueStack.pop_back();
  valueStack.push_back(tc.star(c));
}

ThompsonNFA Parser::rangeToNFA(char start, char end) {
  // TODO Exception
  if (end < start) {
    throw runtime_error("Range out of order.");
  }

  ThompsonNFA tnfa = this->tc.symbol(string(1, start));

  for (char i = start + 1; i <= end; i++) {
    tnfa = this->tc.unionExpression(tnfa, this->tc.symbol(string(1, i)));
  }

  return tnfa;
}

bool Parser::isNextConcated(char currentLetter, char nextLetter) {
  return !(nextLetter == STAR_OP ||         //...*
           nextLetter == OR_OP ||           // ...|
           nextLetter == RIGHT_BRACKET ||   // ...)
           currentLetter == LEFT_BRACKET || // (..
           currentLetter == OR_OP           // |...
  );
};

bool Parser::reduceOpsStack(vector<ThompsonNFA> &valueStack,
                            vector<char> &ops) {
  while (ops.size()) {
    auto top = ops.back();
    switch (top) {
    case OR_OP:
      ops.pop_back();
      this->runUnionOp(valueStack);
      break;
    case VIRTUAL_CAT_OP:
      ops.pop_back();
      this->runConcatOp(valueStack);
      break;
    case STAR_OP:
      ops.pop_back();
      this->runStarOp(valueStack);
      break;
    case LEFT_BRACKET: // touch the bottom
      return true;
    }
  }

  return false;
};

void Parser::pushOp(char opLetter, vector<ThompsonNFA> &valueStack,
                    vector<char> &ops) {

  // before push OP, compare the priority to reduce op stack.
  if (ops.size()) {
    auto top = ops.back();
    auto findedItem = OP_PRIORITY_MAP.find(top);
    if (findedItem != OP_PRIORITY_MAP.end()) {
      if (OP_PRIORITY_MAP[opLetter] <
          findedItem->second) { // op in stack has a bigger priority
        this->reduceOpsStack(valueStack, ops);
      }
    }
  }

  ops.push_back(opLetter);
}

// TODO error situations
// infix situation
// Shunting-yard algorithm
// https://en.wikipedia.org/wiki/Shunting-yard_algorithm
ThompsonNFA Parser::parse(string regExp) {
  int regExpSize = regExp.size();
  if (regExpSize == 0) {
    return tc.emptyExpression();
  }

  vector<ThompsonNFA> valueStack;
  vector<char> ops;

  int index = 0;

  while (index < regExpSize) {
    char letter = regExp[index];
    switch (letter) {
    case RANGE_START:
      // TODO Exception checking
      if (index >= regExpSize - 4 || regExp[index + 2] != RANGE_MID ||
          regExp[index + 4] != RANGE_END) {
        throw runtime_error("Uncorrect range. Range grammer is like [0-9].");
      }

      valueStack.push_back(
          this->rangeToNFA(regExp[index + 1], regExp[index + 3]));

      index += 4; // forward
      break;
    case OR_OP:
      this->pushOp(letter, valueStack, ops);
      break;
    case STAR_OP:
      this->pushOp(letter, valueStack, ops);
      break;
    case LEFT_BRACKET:
      ops.push_back(letter);
      break;
    case RIGHT_BRACKET:
      // pop ops until meeting left bracket
      if (!this->reduceOpsStack(valueStack, ops)) {
        throw runtime_error("bracket '()' does not close correctly.");
      } else {
        ops.pop_back(); // pop LEFT_BRACKET
        // continue to reduce until meet LEFT_BRACKET
        this->reduceOpsStack(valueStack, ops);
      }
      break;
    default:
      valueStack.push_back(tc.symbol(string(1, letter)));
      break;
    }

    // consider concat operation
    if (index < regExpSize - 1) { // if exists more letters
      char nextLetter = regExp[index + 1];
      if (this->isNextConcated(letter, nextLetter)) {
        ops.push_back(VIRTUAL_CAT_OP);
      }
    }

    index++;
  }

  // if still remains some ops
  this->reduceOpsStack(valueStack, ops);

  return valueStack.back();
}

} // namespace sfsm
