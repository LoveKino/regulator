#pragma once

#include <iostream>
#include <unordered_map>

using namespace std;

namespace sfsm {

/**
 * graph = <V, E>
 * state transition: state 1 -> ch -> state 2
 */
class DFA {
  typedef unsigned int DFA_STATE; // use number to stand for DFA state
  typedef unordered_map<char, DFA_STATE> TransitionMap;
  typedef unordered_map<DFA_STATE, TransitionMap> TransitionGraph;

private:
  TransitionGraph transitionGraph;

public:
  // using 0 as start state
  DFA();

  void addTransition(DFA_STATE from, char letter, DFA_STATE to);

  /**
   * @return
   *   -1, stands for not transition
   *   other, target transition
   */
  int transit(DFA_STATE from, char letter);
  string toString();
  void display();

  bool operator==(DFA &other);
  bool operator!=(DFA &other);
};
} // namespace sfsm
