#pragma once

#include "condition.h"
#include <iostream>
#include <vector>

using namespace std;

namespace sfsm {

enum TRANSITION_RESULT_TYPES { WAIT, MATCH, QUIT };
enum STATE_NODE_TYPES { ACCEPT, MIDDLE };

/**
 * Users can use those apis to compose a graph which can be used to build a fsm.
 */
class State_Node { // recoding the relationship between nodes in FSM
public:
  typedef pair<Condition *, State_Node *> Transition;

private:
  STATE_NODE_TYPES type;
  vector<Transition> transitions; // transitions from current state

public:
  State_Node();
  State_Node(STATE_NODE_TYPES type);
  STATE_NODE_TYPES getType();
  void addTransition(Condition *action, State_Node *node);
  void setType(STATE_NODE_TYPES type);
  bool isEnd();
  vector<Transition> getTransitions();
  State_Node *findTargetState(long sign);
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
  TRANSITION_RESULT_TYPES transit(long sign);
};

} // namespace sfsm
