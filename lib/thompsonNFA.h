#pragma once

#include "nfa.h"

using namespace std;

namespace sfsm {
/**
 * a special type of NFA
 */
class ThompsonNFA {
private:
  NFA nfa;
  NFA::NFA_STATE start;
  NFA::NFA_STATE end;

public:
  ThompsonNFA(NFA nfa, NFA::NFA_STATE start, NFA::NFA_STATE end);

  NFA &getNFA();
  NFA::NFA_STATE getStart();
  NFA::NFA_STATE getEnd();
};
} // namespace sfsm
