#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

namespace sfsm {

enum TRANSITION_RESULT_TYPES { WAIT, MATCH, QUIT };
enum STATE_NODE_TYPES { ACCEPT, MIDDLE };

class Action {
public:
  bool match(void *key);
};

/**
 * Users can use those apis to compose a graph which can be used to build a fsm.
 */
class State_Node { // recoding the relationship between nodes in FSM

  struct Transition {
    string condition;
    State_Node *targetState;
  };

private:
  STATE_NODE_TYPES type;
  vector<Transition> transitions; // transitions from current state

public:
  State_Node();
  State_Node(STATE_NODE_TYPES type);
  STATE_NODE_TYPES getType();
  void addTransition(string action, State_Node *node);
  State_Node *findTargetState(string action);
};

/**
 * FSM
 */
class FSM {
private:
  State_Node *current_state; // record current state
  TRANSITION_RESULT_TYPES current_state_type;

public:
  FSM(State_Node *node);
  /**
   * when accept an action, change the state of DFA, and return the new state
   * situation.
   */
  TRANSITION_RESULT_TYPES transit(string sign);
};

/**
 * build a fsm from a state map
 */
FSM *fsm(State_Node *node);
} // namespace sfsm
