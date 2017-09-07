#include "fsm.h"
#include <iostream>

using namespace std;

namespace sfsm {
FSM *fsm(State_Node *node) {
  cout << "calling fsm" << endl;
  return new FSM(node);
}

FSM::FSM(State_Node *node) {
  this->current_state = node;
  this->current_state_type = WAIT;
}

TRANSITION_RESULT_TYPES FSM::transit(string action) {
  if (this->current_state_type != QUIT) {
    State_Node *next = this->current_state->findTargetState(action);
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

State_Node::State_Node() { this->type = MIDDLE; }

State_Node::State_Node(STATE_NODE_TYPES type) { this->type = type; }

STATE_NODE_TYPES State_Node::getType() { return this->type; }

void State_Node::addTransition(string action, State_Node *node) {
  if (this->transitions.find(action) !=
      this->transitions.end()) { // already exists
    throw runtime_error("already have transition from action " + action);
  }

  // add to transitions
  this->transitions[action] = node;
}

State_Node *State_Node::findTargetState(string action) {
  std::unordered_map<string, State_Node *>::const_iterator search =
      this->transitions.find(action);
  if (search == this->transitions.end()) {
    return NULL;
  }

  return search->second;
}
} // namespace sfsm
