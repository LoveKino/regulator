#pragma once

#include "nfa.h"

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
} // namespace sfsm
