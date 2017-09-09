#pragma once

#include "fsm_state.h"
#include <iostream>
#include <vector>

using namespace std;

namespace sfsm {

typedef pair<State_Node *, State_Node *> RowStates;

RowStates row(Condition *condition);
RowStates row(Condition *condition, STATE_NODE_TYPES type);
RowStates row(vector<Condition *> conditionList);
RowStates row(vector<Condition *> conditionList, STATE_NODE_TYPES type);
RowStates row(string str);
RowStates row(string str, STATE_NODE_TYPES type);

// connect two parts with condition
RowStates connect(Condition *c1, Condition *c2);
RowStates connect(RowStates row1, Condition *c);
RowStates connect(RowStates row1, Condition *c, RowStates row2);

/**
 * build a fsm from a state map
 */
FSM *fsm(State_Node *node);
FSM *fsm(RowStates rowState);

} // namespace sfsm
