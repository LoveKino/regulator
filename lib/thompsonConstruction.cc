#include "parser.h"

namespace sfsm {

ThompsonConstruction::ThompsonConstruction() { this->stateCount = 0; }

unsigned int ThompsonConstruction::getNewState() {
  auto v = this->stateCount;

  this->stateCount++;

  return v;
}

ThompsonNFA ThompsonConstruction::emptyExpression() {
  NFA nfa;
  auto state = getNewState();
  return ThompsonNFA(nfa, state, state);
}

ThompsonNFA ThompsonConstruction::fracture() {
  NFA nfa;
  auto start = getNewState();
  auto end = getNewState();
  return ThompsonNFA(nfa, start, end);
}

ThompsonNFA ThompsonConstruction::symbol(char letter) {
  NFA nfa;
  auto start = getNewState();
  auto end = getNewState();

  nfa.addTransition(start, letter, end);

  return ThompsonNFA(nfa, start, end);
}

void ThompsonConstruction::mergeNFA(NFA &nfa1, NFA &n2) {
  auto n2Graph = n2.getTransitionGraph();
  auto n2Eps = n2.getEpsilonTransitions();
  // merge exist nfas
  nfa1.getTransitionGraph().insert(n2Graph.begin(), n2Graph.end());
  nfa1.getEpsilonTransitions().insert(n2Eps.begin(), n2Eps.end());
}

ThompsonNFA ThompsonConstruction::unionExpression(ThompsonNFA n1,
                                                  ThompsonNFA n2) {
  NFA nfa = n1.getNFA();
  auto start = getNewState();
  auto end = getNewState();

  this->mergeNFA(nfa, n2.getNFA());

  nfa.addEpsilonTransition(start, n1.getStart());
  nfa.addEpsilonTransition(start, n2.getStart());

  nfa.addEpsilonTransition(n1.getEnd(), end);
  nfa.addEpsilonTransition(n2.getEnd(), end);

  return ThompsonNFA(nfa, start, end);
}

ThompsonNFA ThompsonConstruction::concatExpression(ThompsonNFA n1,
                                                   ThompsonNFA n2) {
  NFA nfa = n1.getNFA();
  this->mergeNFA(nfa, n2.getNFA());
  nfa.addEpsilonTransition(n1.getEnd(), n2.getStart());

  return ThompsonNFA(nfa, n1.getStart(), n2.getEnd());
}

ThompsonNFA ThompsonConstruction::star(ThompsonNFA n) {
  auto start = this->getNewState();
  auto end = this->getNewState();
  auto nfa = n.getNFA();

  nfa.addEpsilonTransition(start, n.getStart());
  nfa.addEpsilonTransition(start, end);
  nfa.addEpsilonTransition(n.getEnd(), n.getStart());
  nfa.addEpsilonTransition(n.getEnd(), end);

  return ThompsonNFA(nfa, start, end);
}
} // namespace sfsm
