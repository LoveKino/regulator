#pragma once

#include <iostream>
#include <unordered_map>

using namespace std;

namespace sfsm {

class DFA {
  typedef unordered_map<string, unsigned int> TransitionMap;
  typedef unordered_map<unsigned int, TransitionMap> TransitionGraph;

private:
  TransitionGraph transitionGraph;

public:
  // using 0 as start state
  DFA();
  void addTransition(unsigned int from, string letter, unsigned int to);

  void display(); // for debug

  int transit(unsigned int from, string letter);

  bool operator==(DFA &other);
  bool operator!=(DFA &other);
};
} // namespace sfsm
