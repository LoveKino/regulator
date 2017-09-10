#pragma once

#include "fsm_state.h"
#include <iostream>
#include <vector>

using namespace std;

namespace sfsm {

typedef vector<State_Node *> State_Node_List;
typedef pair<State_Node *, State_Node_List> RowStates; // startNode, endNodes

class StateBox {
private:
  State_Node *start;
  vector<State_Node *> ends;

  void init(Condition *condition);

public:
  StateBox(Condition *condition);
  StateBox(long single);
  StateBox(string str);

  StateBox *row(StateBox *box);
  StateBox *row(string str);
  StateBox *row(Condition *c);
  StateBox *row(long single);

  StateBox *col(StateBox *box);
  StateBox *col(string str);

  State_Node *getStart();
};

StateBox *box(Condition *condition);
StateBox *box(long single);
StateBox *box(string str);

/**
 * build a fsm from a state map
 */
FSM *fsm(State_Node *node);
FSM *fsm(StateBox *StateBox);

} // namespace sfsm
