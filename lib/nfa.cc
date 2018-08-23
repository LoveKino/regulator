#include "nfa.h"
#include <iostream>
#include <map>

namespace sfsm {
NFA::NFA() {}

// init-closure(a) = {a}
void NFA::initStateClosure(NFA::NFA_STATE state) {
  if (!this->epsilonClosureMap[state].size()) {
    this->epsilonClosureMap[state].insert(state);
  }
}

void NFA::addTransition(NFA::NFA_STATE from, char letter, NFA::NFA_STATE to) {
  this->transitionGraph[from][letter].insert(to);

  this->initStateClosure(from);
  this->initStateClosure(to);
}

void NFA::addEpsilonTransition(NFA::NFA_STATE from, NFA::NFA_STATE to) {
  // add to epsilon transitions
  this->epsilonToTransitions[from].insert(to);

  // add to epsilon from transitions
  this->epsilonFromTransitions[to].insert(from);

  // epsilon closure map
  this->initStateClosure(from);
  this->initStateClosure(to);

  unordered_set<NFA::NFA_STATE> recordMap;
  this->deliveryClosure(from, this->epsilonClosureMap[to], recordMap);
}

void NFA::deliveryClosure(NFA::NFA_STATE from, NFA_State_Set &toClosure,
                          unordered_set<NFA::NFA_STATE> &passSet) {
  if (passSet.find(from) == passSet.end()) {
    this->epsilonClosureMap[from].insert(toClosure.begin(), toClosure.end());
    passSet.insert(from);

    auto prevs = this->epsilonFromTransitions[from];
    for (auto it = prevs.begin(); it != prevs.end(); ++it) {
      this->deliveryClosure(*it, toClosure, passSet);
    }
  }
}

NFA::TransitionMap NFA::getTransitionMap(NFA::NFA_STATE from) {
  TransitionMap result;
  TransitionGraph::iterator findedItem = this->transitionGraph.find(from);
  if (findedItem != this->transitionGraph.end()) {
    return findedItem->second;
  }

  return result;
}

NFA::NFA_State_Set NFA::transit(NFA::NFA_STATE from, char letter) {
  NFA_State_Set result;
  TransitionMap map = this->getTransitionMap(from);

  TransitionMap::iterator findedTar = map.find(letter);
  if (findedTar != map.end()) {
    // merge set
    return findedTar->second;
  }

  return result;
}

NFA::NFA_State_Set NFA::epsilonClosure(NFA_State_Set &nfaSet) {
  NFA_State_Set result;
  for (auto i = nfaSet.begin(); i != nfaSet.end(); ++i) {
    auto item = this->epsilonClosureMap[*i];
    result.insert(item.begin(), item.end());
  }

  return result;
}

NFA::TransitionMap NFA::getNFASetTransitionMap(NFA_State_Set &stateSet) {
  TransitionMap newMap;
  for (auto stateP = stateSet.begin(); stateP != stateSet.end(); ++stateP) {
    auto map = this->getTransitionMap(*stateP);
    // add map in newMap
    for (auto mapItem = map.begin(); mapItem != map.end(); ++mapItem) {
      newMap[mapItem->first].insert(mapItem->second.begin(),
                                    mapItem->second.end());
    }
  }
  return newMap;
}

pair<DFA, NFA::DFA_StateNFA_SET_MAP> NFA::toDFA(NFA::NFA_STATE startState) {
  // cache
  // TODO instance-level cache
  map<NFA_State_Set, NFA_State_Set> epsilonNFASetCache;

  NFA_State_Set startOrigin{startState};
  auto start = this->epsilonClosure(startOrigin);
  epsilonNFASetCache[startOrigin] = start;

  DFA dfa;
  DFA_StateNFA_SET_MAP stateMap; // record NFA -> DFA details

  map<NFA::NFA_State_Set, NFA::NFA_STATE> dfaStates; // store (set, state)
  vector<NFA::NFA_State_Set> newAdded;
  NFA::NFA_STATE last = 0;
  unsigned int offset = 0;

  dfaStates[start] = 0; // default, the start state is 0
  stateMap[0] = start;
  newAdded.push_back(start);

  while (newAdded.size()) {
    vector<NFA::NFA_State_Set> newAddedTmp;

    unsigned int index = 0;
    for (auto it = newAdded.begin(); it != newAdded.end(); ++it, ++index) {
      auto stateSet = *it;

      TransitionMap newMap = this->getNFASetTransitionMap(stateSet);

      unsigned int currentDFAState = index + offset;
      // for each input, try to find new to-state
      for (auto newItemP = newMap.begin(); newItemP != newMap.end();
           ++newItemP) {
        char letter = newItemP->first;

        NFA_State_Set newItem;
        if (epsilonNFASetCache.find(newItemP->second) != epsilonNFASetCache.end()) {
          newItem = epsilonNFASetCache[newItemP->second];
        } else {
          newItem = this->epsilonClosure(newItemP->second);
          epsilonNFASetCache[newItemP->second] = newItem;
        }

        auto findedItem = dfaStates.find(newItem);
        if (findedItem == dfaStates.end()) { // not exist
          // finded new item
          last++;
          dfaStates[newItem] = last;
          stateMap[last] = newItem;
          newAddedTmp.push_back(newItem);
          // build the connection from (index + offset) -> last
          dfa.addTransition(currentDFAState, letter, last);
        } else { // item exists, just set transition
          dfa.addTransition(currentDFAState, letter, findedItem->second);
        }
      }
    }

    offset += newAdded.size();
    newAdded = newAddedTmp;
  }

  return pair<DFA, DFA_StateNFA_SET_MAP>(dfa, stateMap);
}

void NFA::mergeNFA(NFA &n) {
  auto n2Graph = n.transitionGraph;
  auto n2ToEps = n.epsilonToTransitions;

  // copy transitions
  for (auto i = n2Graph.begin(); i != n2Graph.end(); ++i) {
    int from = i->first;
    auto transitionMap = i->second;
    for (auto j = transitionMap.begin(); j != transitionMap.end(); ++j) {
      char letter = j->first;
      NFA_State_Set toSet = j->second;
      for (auto k = toSet.begin(); k != toSet.end(); ++k) {
        int to = *k;
        this->addTransition(from, letter, to);
      }
    }
  }

  // copy epsilon transitions
  for (auto i = n2ToEps.begin(); i != n2ToEps.end(); ++i) {
    int from = i->first;
    NFA_State_Set toSet = i->second;
    for (auto j = toSet.begin(); j != toSet.end(); ++j) {
      int to = *j;
      this->addEpsilonTransition(from, to);
    }
  }
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
      char letter = j->first;
      NFA_State_Set to = j->second;
      cout << from << "(" << letter << ")"
           << "->";
      this->displayNFA_State_set(to);
    }
  }

  for (auto j = this->epsilonToTransitions.begin();
       j != this->epsilonToTransitions.end(); ++j) {
    int from = j->first;
    NFA_State_Set to = j->second;
    cout << from << "(EPSILON)->";
    this->displayNFA_State_set(to);
  }
}
} // namespace sfsm
