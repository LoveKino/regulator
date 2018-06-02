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
  typedef unordered_map<char, NFA_State_Set> TransitionMap;
  typedef unordered_map<unsigned int, TransitionMap> TransitionGraph;
  typedef unordered_map<unsigned int, NFA_State_Set> EpsilonListMap;
  typedef unordered_map<unsigned int, NFA_State_Set> DFA_StateNFA_SET_MAP;

private:
  TransitionGraph transitionGraph;
  EpsilonListMap epsilonToTransitions; // state ε-> {state}

  // cache
  EpsilonListMap epsilonFromTransitions; // state ->ε {state}
  EpsilonListMap epsilonClosureMap;

  void initStateClosure(unsigned int state);
  void deliveryClosure(unsigned int from, NFA_State_Set &toClosure,
                       unordered_set<unsigned int> &passSet);
  TransitionMap getNFASetTransitionMap(NFA_State_Set &nfaSet);

public:
  // using 0 as start state
  NFA();
  TransitionMap getTransitionMap(unsigned int from);

  void addTransition(unsigned int from, char letter, unsigned int to);
  void addEpsilonTransition(unsigned int from, unsigned int to);

  void mergeNFA(NFA &n);

  NFA_State_Set transit(unsigned int from, char letter);

  // epsilon closure
  NFA_State_Set epsilonClosure(NFA_State_Set &set);

  // convert to DFA
  pair<DFA, DFA_StateNFA_SET_MAP> toDFA(unsigned int start);
  pair<DFA, DFA_StateNFA_SET_MAP> toDFA2();

  // for test
  void displayNFA_State_set(NFA_State_Set set);

  void display();
};
} // namespace sfsm
