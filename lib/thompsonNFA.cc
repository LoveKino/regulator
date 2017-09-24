#include "thompsonNFA.h"

namespace sfsm {

ThompsonNFA::ThompsonNFA(NFA nfa, unsigned int start, unsigned int end) {
  this->nfa = nfa;
  this->start = start;
  this->end = end;
}

NFA &ThompsonNFA::getNFA() { return this->nfa; }

unsigned int ThompsonNFA::getStart() { return this->start; }

unsigned int ThompsonNFA::getEnd() { return this->end; }

} // namespace sfsm
