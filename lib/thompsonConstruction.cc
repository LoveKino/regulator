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

ThompsonNFA ThompsonConstruction::unionExpression(ThompsonNFA n1,
                                                  ThompsonNFA n2) {
  auto start = getNewState();
  auto end = getNewState();

  NFA nfa = n1.getNFA();

  nfa.mergeNFA(n2.getNFA());

  nfa.addEpsilonTransition(start, n1.getStart());
  nfa.addEpsilonTransition(start, n2.getStart());

  nfa.addEpsilonTransition(n1.getEnd(), end);
  nfa.addEpsilonTransition(n2.getEnd(), end);

  return ThompsonNFA(nfa, start, end);
}

ThompsonNFA ThompsonConstruction::unionExpression(vector<ThompsonNFA> list) {
  auto start = getNewState();
  auto end = getNewState();

  NFA nfa;
  bool initFlag = true;

  for (auto it = list.begin(); it != list.end(); ++it) {
    auto next = *it;

    if (initFlag) {
      nfa = next.getNFA();
      initFlag = false;
    } else {
      nfa.mergeNFA(next.getNFA());
    }

    nfa.addEpsilonTransition(start, it->getStart());
    nfa.addEpsilonTransition(it->getEnd(), end);
  }

  return ThompsonNFA(nfa, start, end);
}

ThompsonNFA ThompsonConstruction::unionExpression(unordered_set<char> list) {
  auto start = getNewState();
  auto end = getNewState();

  NFA nfa;

  for (auto it = list.begin(); it != list.end(); ++it) {
    auto letter = *it;
    nfa.addTransition(start, letter, end);
  }

  return ThompsonNFA(nfa, start, end);
}

ThompsonNFA ThompsonConstruction::concatExpression(ThompsonNFA n1,
                                                   ThompsonNFA n2) {
  NFA nfa = n1.getNFA();
  nfa.mergeNFA(n2.getNFA());
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
