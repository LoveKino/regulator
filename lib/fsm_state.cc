#include "fsm_state.h"
#include <iostream>

using namespace std;

namespace sfsm {
/**
 * State_Node
 */
State_Node::State_Node() { this->type = MIDDLE; }

State_Node::State_Node(STATE_NODE_TYPES type) { this->type = type; }

STATE_NODE_TYPES State_Node::getType() { return this->type; }

void State_Node::setType(STATE_NODE_TYPES type) { this->type = type; }

bool State_Node::isEnd() { return this->transitions.size() == 0; }

vector<State_Node::Transition> State_Node::getTransitions() {
  return this->transitions;
}

void State_Node::addTransition(Condition *condition, State_Node *node) {
  // TODO check repeatness
  // add to transitions
  this->transitions.push_back(make_pair(condition, node));
}

State_Node *State_Node::findTargetState(long sign) {
  // search by action
  for (vector<Transition>::iterator it = this->transitions.begin();
       it != this->transitions.end(); ++it) {
    if (it->first->match(sign)) { // compare action
      return it->second;
    }
  }

  return NULL;
}

/**
 * FSM
 */
FSM::FSM(State_Node *node) {
  this->current_state = node;
  this->current_state_type = WAIT;
}

/**
 * transit from a sign, return the current state type
 */
TRANSITION_RESULT_TYPES FSM::transit(long sign) {
  if (this->current_state_type != QUIT) {
    // match by sign
    State_Node *next = this->current_state->findTargetState(sign);

    // change current state
    if (next == NULL) {
      this->current_state_type = QUIT;
      this->current_state = NULL;
    } else {
      this->current_state = next; // change current state

      // consider the state type
      if (next->getType() == ACCEPT) { // some states marked ACCEPT explicitly
        this->current_state_type = MATCH;
      } else {
        // some states using default type (middle)
        // if the state's out-degree is 0, regrard this state as ACCEPT state.
        if (this->current_state->isEnd()) {
          this->current_state_type = MATCH;
        } else {
          this->current_state_type = WAIT;
        }
      }
    }
  }

  return this->current_state_type;
}

} // namespace sfsm
