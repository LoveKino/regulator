#include "dfa.h"
#include <iostream>

using namespace std;

namespace sfsm {
DFA::DFA() {}

void DFA::addTransition(DFA::DFA_STATE from, char letter, DFA::DFA_STATE to) {
  auto it = this->transitionGraph.find(from);
  if (it == this->transitionGraph.end()) {
    TransitionMap map;
    this->transitionGraph[from] = map;
  }
  this->transitionGraph[from][letter] = to;
}

string DFA::toString() {
  string text = "";
  for (auto i = this->transitionGraph.begin(); i != this->transitionGraph.end();
       ++i) {
    int from = i->first;
    TransitionMap map = i->second;
    for (auto j = map.begin(); j != map.end(); ++j) {
      char letter = j->first;
      int to = j->second;
      string line = std::to_string(from) + "(" + letter + ")" + "->" + std::to_string(to);
      if(text.size() == 0) {
        text += line;
      } else {
        text += "\n" + line;
      }
    }
  }

  return text;
}

void DFA::display() {
  cout << this -> toString();
}

int DFA::transit(DFA::DFA_STATE from, char letter) {
  auto it = this->transitionGraph.find(from);
  if (it == this->transitionGraph.end()) {
    return -1;
  }
  auto transitionMap = this->transitionGraph[from];
  auto tar = transitionMap.find(letter);
  if (tar == transitionMap.end()) {
    return -1;
  }

  return transitionMap[letter];
}

bool DFA::operator==(DFA &other) {
  return this->transitionGraph == other.transitionGraph;
}

bool DFA::operator!=(DFA &other) { return !(*this == other); }
} // namespace sfsm
