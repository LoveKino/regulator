#include "thompsonNFA.h"

namespace sfsm {

ThompsonNFA::ThompsonNFA(NFA nfa, NFA::NFA_STATE start, NFA::NFA_STATE end) {
  this->nfa = nfa;
  this->start = start;
  this->end = end;
}

NFA &ThompsonNFA::getNFA() { return this->nfa; }

NFA::NFA_STATE ThompsonNFA::getStart() { return this->start; }

NFA::NFA_STATE ThompsonNFA::getEnd() { return this->end; }

} // namespace sfsm
