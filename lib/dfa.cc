#include "dfa.h"
#include <iostream>

using namespace std;

namespace sfsm {
DFA::DFA() {}

void DFA::addTransition(unsigned int from, string letter, unsigned int to) {
  auto it = this->transitionGraph.find(from);
  if (it == this->transitionGraph.end()) {
    TransitionMap map;
    this->transitionGraph[from] = map;
  }
  this->transitionGraph[from][letter] = to;
}

void DFA::display() {
  for (auto i = this->transitionGraph.begin(); i != this->transitionGraph.end();
       ++i) {
    int from = i->first;
    TransitionMap map = i->second;
    for (auto j = map.begin(); j != map.end(); ++j) {
      string letter = j->first;
      int to = j->second;
      cout << from << "(" << letter << ")"
           << "->" << to << endl;
    }
  }
}

bool DFA::operator==(DFA &other) {
  return this->transitionGraph == other.transitionGraph;
}

bool DFA::operator!=(DFA &other) { return !(*this == other); }
} // namespace sfsm
