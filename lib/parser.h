#pragma once

#include "unordered_map"
#include "thompsonNFA.h"
#include "thompsonConstruction.h"

using namespace std;

namespace sfsm {
class Parser {
private:
  ThompsonConstruction tc;

  // special symbols
  const static char OR_OP;
  const static char STAR_OP;
  const static char VIRTUAL_CAT_OP;
  const static char RIGHT_BRACKET;
  const static char LEFT_BRACKET;
  const static char RANGE_START;
  const static char RANGE_MID;
  const static char RANGE_END;

  static unordered_map<char, int> OP_PRIORITY_MAP; // priority map

  bool isNormalLetter(char letter);
  void runUnionOp(vector<ThompsonNFA> &valueStack);
  void runConcatOp(vector<ThompsonNFA> &valueStack);
  void runStarOp(vector<ThompsonNFA> &valueStack);
  bool isNextConcated(char currentLetter, char nextLetter);
  void pushOp(char opLetter, vector<ThompsonNFA> &valueStack,
              vector<char> &ops);

  // if meet LEFT_BRACKET return true, otherwise return false.
  bool reduceOpsStack(vector<ThompsonNFA> &valueStack, vector<char> &ops);

  // range
  ThompsonNFA rangeToNFA(char start, char end);

public:
  ThompsonNFA parse(string regExp);
};
} // namespace sfsm
