#pragma once

#include "dfa.h"
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

namespace sfsm {
/**
 * NFA Graph = <V, E>
 * state -> ch -> Set[state]
 */
class NFA {
public:
  typedef unsigned int NFA_STATE;
  typedef set<NFA_STATE> NFA_State_Set;
  typedef unordered_map<char, NFA_State_Set> TransitionMap;
  typedef unordered_map<NFA_STATE, TransitionMap> TransitionGraph;
  typedef unordered_map<NFA_STATE, NFA_State_Set> EpsilonListMap;
  typedef unordered_map<NFA_STATE, NFA_State_Set> DFA_StateNFA_SET_MAP;

private:
  TransitionGraph transitionGraph;
  EpsilonListMap epsilonToTransitions; // state ε-> {state}

  // cache
  EpsilonListMap epsilonFromTransitions; // state ->ε {state}
  EpsilonListMap epsilonClosureMap;

  void initStateClosure(NFA_STATE state);
  void deliveryClosure(NFA_STATE from, NFA_State_Set &toClosure,
                       unordered_set<NFA_STATE> &passSet);
  TransitionMap getNFASetTransitionMap(NFA_State_Set &nfaSet);

public:
  // using 0 as start state
  NFA();
  TransitionMap getTransitionMap(NFA_STATE from);

  void addTransition(NFA_STATE from, char letter, NFA_STATE to);
  void addEpsilonTransition(NFA_STATE from, NFA_STATE to);

  void mergeNFA(NFA &n);

  NFA_State_Set transit(NFA_STATE from, char letter);

  // epsilon closure
  NFA_State_Set epsilonClosure(NFA_State_Set &set);

  // convert to DFA
  pair<DFA, DFA_StateNFA_SET_MAP> toDFA(NFA_STATE start);

  // for test
  void displayNFA_State_set(NFA_State_Set set);

  void display();
};
} // namespace sfsm
