#pragma once

#include "dfa.h"
#include "nfa.h"
#include <iostream>

using namespace std;

namespace sfsm {
class ThompsonNFA {
private:
  NFA nfa;
  unsigned int start;
  unsigned int end;

public:
  ThompsonNFA(NFA nfa, unsigned int start, unsigned int end);

  NFA &getNFA();
  unsigned int getStart();
  unsigned int getEnd();
};

class ThompsonConstruction {
private:
  unsigned int stateCount;
  unsigned int getNewState();

  void mergeNFA(NFA &nfa1, NFA &nfa2);

public:
  ThompsonConstruction();
  ThompsonNFA emptyExpression();
  ThompsonNFA symbol(string letter);
  ThompsonNFA unionExpression(ThompsonNFA n1, ThompsonNFA n2);
  ThompsonNFA concatExpression(ThompsonNFA n1, ThompsonNFA n2);
  ThompsonNFA star(ThompsonNFA n);
};

class Parser {
private:
  ThompsonConstruction tc;
  static char OR_OP;
  static char RIGHT_BRACKET;
  static char LEFT_BRACKET;
  static char VIRTUAL_CAT_OP;

  bool isNormalLetter(char letter);
  void runUnionOp(vector<ThompsonNFA> &valueStack);
  void runConcatOp(vector<ThompsonNFA> &valueStack);
  bool isNextConcated(char currentLetter, char nextLetter);

  // if meet LEFT_BRACKET return true, otherwise return false.
  bool reduceOpsStack(vector<ThompsonNFA> &valueStack, vector<char> &ops);

public:
  ThompsonNFA parse(string regExp);
};
} // namespace sfsm
