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

void State_Node::clear() { this->transitions.clear(); }

// copy all transitions of n
void State_Node::copyNodeTransitions(State_Node *n) {
  vector<State_Node::Transition>::iterator it;
  vector<State_Node::Transition> list = n->getTransitions();

  for (it = list.begin(); it != list.end(); ++it) {
    this->addTransition(it->first, it->second);
  }
};

vector<State_Node::Transition> State_Node::getTransitions() {
  return this->transitions;
}

void State_Node::addTransition(Condition *condition, State_Node *node) {
  // TODO check repeatness
  // add to transitions
  this->transitions.push_back(make_pair(condition, node));
}

vector<State_Node *> State_Node::findTargetState(long sign) {
  vector<State_Node *> ret;
  // search by action
  for (vector<Transition>::iterator it = this->transitions.begin();
       it != this->transitions.end(); ++it) {
    // may have more than one matching
    if (it->first->match(sign)) { // compare action
      ret.push_back(it->second);
    }
  }

  return ret;
}

/**
 * FSM
 */
FSM::FSM(State_Node *startNode) {
  vector<State_Node *> list;
  list.push_back(startNode);

  this->current_states = list;
  this->current_state_type = WAIT;
}

/**
 * transit from a sign, return the current state type
 */
TRANSITION_RESULT_TYPES FSM::transit(long sign) {
  if (this->current_state_type != QUIT) {
    vector<State_Node *>::iterator it;
    bool hasMatch = false;
    bool hasWait = false;

    vector<State_Node *> newList;

    for (it = this->current_states.begin(); it != this->current_states.end();
         ++it) {
      // match by sign
      vector<State_Node *> next = (*it)->findTargetState(sign);
      vector<State_Node *>::iterator nit;
      for (nit = next.begin(); nit != next.end(); ++nit) {
        State_Node *nextItem = *nit;
        newList.push_back(nextItem); // as one of current states
        // consider the state type
        // some states marked ACCEPT explicitly
        // some states using default type (middle)
        // if the state's out-degree is 0, regrard this state as ACCEPT state.
        if (nextItem->getType() == ACCEPT || nextItem->isEnd()) {
          hasMatch = true;
        } else {
          hasWait = true;
        }
      }
    }

    this->current_states.clear();
    this->current_states = newList;

    if (hasMatch) {
      this->current_state_type = MATCH;
    } else if (hasWait) {
      this->current_state_type = WAIT;
    } else {
      this->current_state_type = QUIT;
    }
  }

  return this->current_state_type;
} // namespace sfsm

} // namespace sfsm
