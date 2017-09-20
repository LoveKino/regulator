#pragma once

#include "dfa.h"
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

namespace sfsm {
class NFA {
public:
  typedef set<unsigned int> NFA_State_Set;
  typedef unordered_map<string, NFA_State_Set> TransitionMap;
  typedef unordered_map<unsigned int, TransitionMap> TransitionGraph;
  typedef unordered_map<unsigned int, NFA_State_Set> EpsilonListMap;

private:
  TransitionGraph transitionGraph;
  EpsilonListMap epsilonTransitions;

public:
  // using 0 as start state
  NFA();
  TransitionGraph &getTransitionGraph();
  EpsilonListMap &getEpsilonTransitions();
  TransitionMap getTransitionMap(unsigned int from);

  void addTransition(unsigned int from, string letter, unsigned int to);
  void addEpsilonTransition(unsigned int from, unsigned int to);

  NFA_State_Set transit(unsigned int from, string letter);

  // get new nfa states set by letter
  NFA_State_Set groupTransist(NFA_State_Set set, string letter);

  // get input set from a set of nfa
  unordered_set<string> getInputSet(NFA_State_Set set);

  // epsilon closure
  NFA_State_Set epsilonClosure(NFA_State_Set set);

  //
  string calNFASetId(NFA_State_Set set);

  // convert to DFA
  DFA toDFA(unsigned int start);

  // for test
  void displayNFA_State_set(NFA_State_Set set);

  void display();
};
} // namespace sfsm
