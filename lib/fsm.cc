#include "fsm.h"
#include <iostream>

using namespace std;

namespace sfsm {
FSM *fsm(State_Node *node) {
  cout << "calling fsm" << endl;
  return new FSM(node);
}

/**
 * FSM
 */
FSM::FSM(State_Node *node) {
  this->current_state = node;
  this->current_state_type = WAIT;
}

TRANSITION_RESULT_TYPES FSM::transit(string sign) {
  return this->transit(&sign);
}

TRANSITION_RESULT_TYPES FSM::transit(void *sign) {
  if (this->current_state_type != QUIT) {
    // match by sign
    State_Node *next = this->current_state->findTargetState(sign);

    // change current state
    if (next == NULL) {
      this->current_state_type = QUIT;
      this->current_state = NULL;
    } else {
      this->current_state = next; // change current state
      this->current_state_type = next->getType() == ACCEPT ? MATCH : WAIT;
    }
  }

  return this->current_state_type;
}

/**
 * State_Node
 */
State_Node::State_Node() { this->type = MIDDLE; }

State_Node::State_Node(STATE_NODE_TYPES type) { this->type = type; }

STATE_NODE_TYPES State_Node::getType() { return this->type; }

void State_Node::addTransition(Condition *condition, State_Node *node) {
  // TODO check repeatness
  // add to transitions
  this->transitions.push_back(make_pair(condition, node));
}

State_Node *State_Node::findTargetState(void *sign) {
  // search by action
  for (vector<Transition>::iterator it = this->transitions.begin();
       it != this->transitions.end(); ++it) {
    if (it->first->match(sign)) { // compare action
      return it->second;
    }
  }

  return NULL;
}
} // namespace sfsm
