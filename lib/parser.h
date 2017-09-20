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
public:
  ThompsonNFA parse(string regExp);
};
} // namespace sfsm
