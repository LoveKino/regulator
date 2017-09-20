#include "nfa.h"
#include <iostream>
#include <map>

namespace sfsm {
NFA::NFA() {}

NFA::TransitionGraph &NFA::getTransitionGraph() {
  return this->transitionGraph;
}

NFA::EpsilonListMap &NFA::getEpsilonTransitions() {
  return this->epsilonTransitions;
}

void NFA::addTransition(unsigned int from, string letter, unsigned int to) {
  if (this->transitionGraph.find(from) == this->transitionGraph.end()) {
    TransitionMap map;
    this->transitionGraph[from] = map;
  }

  if (this->transitionGraph[from].find(letter) ==
      this->transitionGraph[from].end()) {
    NFA_State_Set nfaSet;
    this->transitionGraph[from][letter] = nfaSet;
  }

  this->transitionGraph[from][letter].insert(to);
}

void NFA::addEpsilonTransition(unsigned int from, unsigned int to) {
  if (this->epsilonTransitions.find(from) == this->epsilonTransitions.end()) {
    NFA_State_Set nfaSet;
    this->epsilonTransitions[from] = nfaSet;
  }

  this->epsilonTransitions[from].insert(to);
}

NFA::TransitionMap NFA::getTransitionMap(unsigned int from) {
  TransitionMap result;
  TransitionGraph::iterator findedItem = this->transitionGraph.find(from);
  if (findedItem != this->transitionGraph.end()) {
    return findedItem->second;
  }

  return result;
}

NFA::NFA_State_Set NFA::transit(unsigned int from, string letter) {
  NFA_State_Set result;
  TransitionMap map = this->getTransitionMap(from);

  TransitionMap::iterator findedTar = map.find(letter);
  if (findedTar != map.end()) {
    // merge set
    return findedTar->second;
  }

  return result;
}

NFA::NFA_State_Set NFA::groupTransist(NFA_State_Set nfaSet, string letter) {
  NFA_State_Set result;

  for (auto it = nfaSet.begin(); it != nfaSet.end(); ++it) {
    NFA_State_Set tar = this->transit(*it, letter);
    result.insert(tar.begin(), tar.end());
  }

  return result;
}

unordered_set<string> NFA::getInputSet(NFA_State_Set set) {
  unordered_set<string> result;

  for (auto it = set.begin(); it != set.end(); ++it) {
    unsigned int from = *it;
    TransitionMap map = this->getTransitionMap(from);
    for (auto j = map.begin(); j != map.end(); ++j) {
      result.insert(j->first);
    }
  }

  return result;
}

NFA::NFA_State_Set NFA::epsilonClosure(NFA_State_Set nfaSet) {
  cout << "closure of -------" << endl;
  this->displayNFA_State_set(nfaSet);

  EpsilonListMap epsilonListMap = this->epsilonTransitions;
  unsigned long prevSize = 0;

  while (prevSize < nfaSet.size()) {
    prevSize = nfaSet.size();
    cout << "nfa set------------" << endl;
    this->displayNFA_State_set(nfaSet);

    for (auto fp = nfaSet.begin(); fp != nfaSet.end(); ++fp) {
      auto findedItem = epsilonListMap.find(*fp);

      if (findedItem != epsilonListMap.end()) {
        auto ets = findedItem->second;

        if (ets.size()) {
          for (auto toNfa = ets.begin(); toNfa != ets.end(); ++toNfa) {
            nfaSet.insert(*toNfa);
          }
        }
      }
    }
  }

  return nfaSet;
}

DFA NFA::toDFA(unsigned int startState) {
  NFA::NFA_State_Set start;
  start.insert(startState);
  start = this->epsilonClosure(start);

  DFA dfa;

  map<NFA::NFA_State_Set, int> dfaStates; // store (set, state)
  vector<NFA::NFA_State_Set> newAdded;
  unsigned int last = 0;
  unsigned int offset = 0;

  dfaStates[start] = 0;
  newAdded.push_back(start);

  while (newAdded.size()) {
    vector<NFA::NFA_State_Set> newAddedTmp;

    int index = 0;
    for (auto it = newAdded.begin(); it != newAdded.end(); ++it, ++index) {
      // *it is a closure
      auto inputs = this->getInputSet(*it);
      unsigned int currentDFAState = index + offset;

      for (auto j = inputs.begin(); j != inputs.end(); ++j) {
        auto newItem = this->epsilonClosure(this->groupTransist(*it, *j));
        auto findedItem = dfaStates.find(newItem);

        if (findedItem == dfaStates.end()) { // not exist
          // finded new item
          last++;
          dfaStates[newItem] = last;
          newAddedTmp.push_back(newItem);
          // build the connection from (index + offset) -> last
          dfa.addTransition(currentDFAState, *j, last);
        } else { // item exists, just set transition
          dfa.addTransition(currentDFAState, *j, findedItem->second);
        }
      }
    }

    offset += newAdded.size();
    newAdded = newAddedTmp;
  }

  return dfa;
}

void NFA::displayNFA_State_set(NFA_State_Set set) {
  for (auto it = set.begin(); it != set.end(); ++it) {
    cout << *it << "  ";
  }
  cout << endl;
}

void NFA::display() {
  for (auto i = this->transitionGraph.begin(); i != this->transitionGraph.end();
       ++i) {
    int from = i->first;
    TransitionMap map = i->second;
    for (auto j = map.begin(); j != map.end(); ++j) {
      string letter = j->first;
      NFA_State_Set to = j->second;
      cout << from << "(" << letter << ")"
           << "->";
      this->displayNFA_State_set(to);
    }
  }

  for (auto j = this->epsilonTransitions.begin();
       j != this->epsilonTransitions.end(); ++j) {
    int from = j->first;
    NFA_State_Set to = j->second;
    cout << from << "(EPSILON)->";
    this->displayNFA_State_set(to);
  }
}
} // namespace sfsm
