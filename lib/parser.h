#pragma once

#include "thompsonConstruction.h"
#include "thompsonNFA.h"
#include "unordered_map"
#include "unordered_set"

using namespace std;

namespace sfsm {
class Parser {
public:
  // special symbols
  const static char OR_OP;
  const static char STAR_OP;
  const static char NOT_OP;
  const static char OPTION_OP;
  const static char VIRTUAL_CAT_OP;

  const static char ESCAPE_SYMBOL;

  const static char LEFT_BRACKET;
  const static char RIGHT_BRACKET;
  const static char RANGE_START;
  const static char RANGE_MID;
  const static char RANGE_END;
  typedef unordered_set<char> CharSet;

private:
  ThompsonConstruction tc;

  static unordered_map<char, int> OP_PRIORITY_MAP; // priority map
  static CharSet REG_HOLD_SYMBOLS;

  bool isNormalLetter(char letter);

  void runUnionOp(vector<ThompsonNFA> &valueStack);
  void runConcatOp(vector<ThompsonNFA> &valueStack);
  void runStarOp(vector<ThompsonNFA> &valueStack);
  void runOptionOp(vector<ThompsonNFA> &valueStack);

  bool isNextConcated(char currentLetter, char nextLetter);
  void pushOp(char opLetter, vector<ThompsonNFA> &valueStack,
              vector<char> &ops);

  // if meet LEFT_BRACKET return true, otherwise return false.
  bool reduceOpsStack(vector<ThompsonNFA> &valueStack, vector<char> &ops);

  CharSet getRange(char start, char end);

  // range
  pair<CharSet, unsigned int>
  parseRange(string &regExp, unsigned int index); // regExp[index] is '['

  ThompsonNFA rangeToNFA(CharSet range);

  // negative
  ThompsonNFA notToNFA(CharSet letters);

  // escape
  
  char getEscapedLetter(string &regExp, unsigned int index);

  void throwSyntaxError(string regExp, int errorIndex, string msg);

public:
  ThompsonNFA parse(string regExp);
};
} // namespace sfsm
