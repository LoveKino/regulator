#include "parser.h"

namespace sfsm {
const char Parser::OR_OP = '|';
const char Parser::NOT_OP = '^';
const char Parser::STAR_OP = '*';
const char Parser::OPTION_OP = '?';

const char Parser::ESCAPE_SYMBOL = '\\';

const char Parser::LEFT_BRACKET = '(';
const char Parser::RIGHT_BRACKET = ')';
const char Parser::RANGE_START = '[';
const char Parser::RANGE_MID = '-';
const char Parser::RANGE_END = ']';

const char Parser::VIRTUAL_CAT_OP = '1'; // virtual operation

unordered_map<char, int> Parser::OP_PRIORITY_MAP{
    {'|', 1}, {'?', 4}, {'*', 4}, {'1', 1}};

Parser::CharSet Parser::REG_HOLD_SYMBOLS{
    Parser::OR_OP,       Parser::NOT_OP,       Parser::STAR_OP,
    Parser::OPTION_OP,   Parser::LEFT_BRACKET, Parser::RIGHT_BRACKET,
    Parser::RANGE_START, Parser::RANGE_MID,    Parser::RANGE_END};

bool Parser::isNormalLetter(char letter) {
  return letter == ESCAPE_SYMBOL ||
         REG_HOLD_SYMBOLS.find(letter) == REG_HOLD_SYMBOLS.end();
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

void Parser::runOptionOp(vector<ThompsonNFA> &valueStack) {
  // TODO Exception
  auto c = valueStack.back();
  valueStack.pop_back();
  valueStack.push_back(this->tc.unionExpression(c, this->tc.emptyExpression()));
}

ThompsonNFA Parser::rangeToNFA(CharSet range) {
  return this->tc.unionExpression(range);
}

// char hold 0 - 127
ThompsonNFA Parser::notToNFA(CharSet letters) {
  CharSet list;
  for (int i = 0; i < 128; i++) {
    if (letters.find(i) == letters.end()) {
      list.insert(i);
    }
  }

  return this->tc.unionExpression(list);
}

bool Parser::isNextConcated(char currentLetter, char nextLetter) {
  return !(nextLetter == STAR_OP ||         //...*
           nextLetter == OPTION_OP ||       // ...?
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
    case OPTION_OP:
      ops.pop_back();
      this->runOptionOp(valueStack);
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

Parser::CharSet Parser::getRange(char start, char end) {
  CharSet range;

  if (end < start) {
    throw runtime_error("Range out of order.");
  }

  for (char i = start; i <= end; i++) {
    range.insert(i);
  }

  return range;
}

// [0-9] [abcd] [0-9a-f]
pair<Parser::CharSet, unsigned int>
Parser::parseRange(string &regExp,
                   unsigned int index) { // regExp[index] is '['
  vector<CharSet> ranges;
  int regExpSize = regExp.size();
  int offset = 0;

  index++;
  bool meetEnd = false;

  while (index < regExpSize) {
    if (regExp[index] == RANGE_END) {
      offset++;
      meetEnd = true; // meet end.
      break;
    } else {
      char start = regExp[index];
      if (start == ESCAPE_SYMBOL) {
        start = this->getEscapedLetter(regExp, index);
        index++;
        offset++;
      } else if (!this->isNormalLetter(start)) {
        throwSyntaxError(regExp, index,
                         "Uncorrect range. Range grammer is like [0-9].");
      }

      if (index < regExpSize - 2 && regExp[index + 1] == RANGE_MID) {
        char end = regExp[index + 2];
        if (end == ESCAPE_SYMBOL) {
          end = this->getEscapedLetter(regExp, index);
          index++;
          offset++;
        } else if (!this->isNormalLetter(end)) {
          throwSyntaxError(regExp, index + 2,
                           "Uncorrect range. Range grammer is like [0-9].");
        }

        ranges.push_back(this->getRange(start, end));
        index += 2;
        offset += 2;
      } else {
        ranges.push_back({regExp[index]});
      }
    }

    offset++;
    index++;
  }

  if (!meetEnd) {
    throwSyntaxError(regExp, index,
                     "Uncorrect range. Range grammer is like [0-9].");
  }

  CharSet range;

  for (auto i = ranges.begin(); i != ranges.end(); i++) {
    range.insert(i->begin(), i->end());
  }

  return {range, offset};
}

void Parser::throwSyntaxError(string regExp, int errorIndex, string msg) {
  string prev = errorIndex > 0 ? regExp.substr(max<int>(errorIndex - 5, 0),
                                               errorIndex >= 5 ? 5 : errorIndex)
                               : "";
  string after =
      errorIndex < regExp.size() - 1
          ? regExp.substr(errorIndex + 1,
                          min<int>(regExp.size() - 1 - errorIndex, 5))
          : "";

  throw runtime_error("[Parsing RegExp error], error position " +
                      to_string(errorIndex) + ". Arround string is '" + prev +
                      "  >>>" + regExp[errorIndex] + "<<<  " + after + "'." +
                      msg);
}

char Parser::getEscapedLetter(string &regExp,
                              unsigned int index) { // regExp[index]= '\\'
  int regExpSize = regExp.size();
  if (index >= regExpSize - 1) {
    this->throwSyntaxError(regExp, index, "Missing letter to escape.");
  }

  return regExp[index + 1];
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
  pair<CharSet, unsigned int> partial;

  while (index < regExpSize) {
    char letter = regExp[index];
    switch (letter) {
    case ESCAPE_SYMBOL:
      valueStack.push_back(this->tc.symbol(this->getEscapedLetter(regExp, index)));
      index++;
      break;

    case RANGE_START: // [a-d] TODO more powerful
      // TODO Exception checking
      partial = this->parseRange(regExp, index);
      valueStack.push_back(this->rangeToNFA(partial.first));
      index += partial.second; // forward
      break;

    case NOT_OP: // ^a ^[a-d] TODO more powerful
      if (index >= regExpSize - 1 ||
          (!this->isNormalLetter(regExp[index + 1]) &&
           regExp[index + 1] != RANGE_START)) {
        throw runtime_error("Uncorrect not. Not grammer is like ^a.");
      }

      if (regExp[index + 1] == RANGE_START) {
        index++;
        partial = this->parseRange(regExp, index);
        index += partial.second;
        valueStack.push_back(this->notToNFA(partial.first));
      } else {
        valueStack.push_back(this->notToNFA({regExp[index + 1]}));
        index++; // forward
      }

      break;

    // infix operations
    case OR_OP:
      this->pushOp(letter, valueStack, ops);
      break;
    case STAR_OP:
      this->pushOp(letter, valueStack, ops);
      break;
    case OPTION_OP:
      this->pushOp(letter, valueStack, ops);
      break;

    // brackets
    case LEFT_BRACKET:
      ops.push_back(letter);
      break;
    case RIGHT_BRACKET:
      // pop ops until meeting left bracket
      if (!this->reduceOpsStack(valueStack, ops)) {
        this->throwSyntaxError(regExp, index,
                               "Bracket '()' does not close correctly.");
      } else {
        ops.pop_back(); // pop LEFT_BRACKET
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
        this->pushOp(VIRTUAL_CAT_OP, valueStack, ops);
      }
    }

    index++;
  }

  // if still remains some ops
  this->reduceOpsStack(valueStack, ops);

  return valueStack.back();
} // namespace sfsm

} // namespace sfsm
