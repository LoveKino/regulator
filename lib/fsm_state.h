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
  vector<State_Node *> from;      // record from states

public:
  State_Node();
  State_Node(STATE_NODE_TYPES type);
  STATE_NODE_TYPES getType();
  void addTransition(Condition *action, State_Node *node);
  void setType(STATE_NODE_TYPES type);
  bool isEnd();
  void clear();
  void copyNodeTransitions(State_Node *n);
  vector<Transition> getTransitions();
  vector<State_Node *> getFrom();
  vector<State_Node *> findTargetState(long sign);
};

/**
 * FSM
 */
class FSM {
private:
  vector<State_Node *> current_states; // record current state
  TRANSITION_RESULT_TYPES current_state_type;

public:
  FSM(State_Node *startNode);
  /**
   * when accept an action, change the state of DFA, and return the new state
   * situation.
   */
  TRANSITION_RESULT_TYPES transit(long sign);
};

} // namespace sfsm
