#pragma once

#include "thompsonNFA.h"

using namespace std;

namespace sfsm {
class ThompsonConstruction {
private:
  unsigned int stateCount;
  unsigned int getNewState();

public:
  ThompsonConstruction();
  ThompsonNFA emptyExpression();
  ThompsonNFA fracture();
  ThompsonNFA symbol(char letter);
  ThompsonNFA unionExpression(ThompsonNFA n1, ThompsonNFA n2);
  ThompsonNFA unionExpression(vector<ThompsonNFA> list);
  ThompsonNFA unionExpression(unordered_set<char> list);
  ThompsonNFA concatExpression(ThompsonNFA n1, ThompsonNFA n2);
  ThompsonNFA star(ThompsonNFA n);
};
} // namespace sfsm
