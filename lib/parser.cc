#include "parser.h"

namespace sfsm {
const char Parser::OR_OP = '|';
const char Parser::NOT_OP = '^';
const char Parser::STAR_OP = '*';
const char Parser::LEFT_BRACKET = '(';
const char Parser::RIGHT_BRACKET = ')';
const char Parser::RANGE_START = '[';
const char Parser::RANGE_MID = '-';
const char Parser::RANGE_END = ']';

const char Parser::VIRTUAL_CAT_OP = '1'; // virtual operation

unordered_map<char, int> Parser::OP_PRIORITY_MAP{{'|', 1}, {'*', 4}, {'1', 1}};
unordered_set<char> Parser::REG_HOLD_SYMBOLS{
    Parser::OR_OP,        Parser::NOT_OP,        Parser::STAR_OP,
    Parser::LEFT_BRACKET, Parser::RIGHT_BRACKET, Parser::RANGE_START,
    Parser::RANGE_MID,    Parser::RANGE_END};

bool Parser::isNormalLetter(char letter) {
  return REG_HOLD_SYMBOLS.find(letter) == REG_HOLD_SYMBOLS.end();
}

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

  ThompsonNFA tnfa = this->tc.symbol(start);

  for (char i = start + 1; i <= end; i++) {
    tnfa = this->tc.unionExpression(tnfa, this->tc.symbol(i));
  }

  return tnfa;
}

// char hold 0 - 127
ThompsonNFA Parser::notToNFA(char letter) {
  int start = 0;
  if (letter == 0) {
    start = 1;
  }
  ThompsonNFA tnfa = this->tc.symbol(start);
  for (int i = 0; i < 128; i++) {
    if (letter != i) {
      tnfa = this->tc.unionExpression(tnfa, this->tc.symbol(i));
    }
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
    case RANGE_START: // [a-d] TODO more powerful
      // TODO Exception checking
      if (index >= regExpSize - 4 || regExp[index + 2] != RANGE_MID ||
          regExp[index + 4] != RANGE_END ||
          !this->isNormalLetter(regExp[index + 1]) ||
          !this->isNormalLetter(regExp[index + 3])) {
        throw runtime_error("Uncorrect range. Range grammer is like [0-9].");
      }

      valueStack.push_back(
          this->rangeToNFA(regExp[index + 1], regExp[index + 3]));

      index += 4; // forward
      break;
    case NOT_OP: // ^a TODO more powerful
      if (index >= regExpSize - 1 || !this->isNormalLetter(regExp[index + 1])) {
        throw runtime_error("Uncorrect not. Not grammer is like ^a.");
      }

      valueStack.push_back(this->notToNFA(regExp[index + 1]));

      index++; // forward
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
      valueStack.push_back(tc.symbol(letter));
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
