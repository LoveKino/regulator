#pragma once

#include "thompsonNFA.h"

using namespace std;

namespace sfsm {
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
} // namespace sfsm
