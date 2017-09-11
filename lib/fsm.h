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
  StateBox();
  StateBox(Condition *condition);
  StateBox(long single);
  StateBox(string str);

  StateBox *row(StateBox *box);
  StateBox *row(string str);
  StateBox *row(Condition *c);
  StateBox *row(long single);

  StateBox *col(StateBox *box);
  StateBox *col(string str);

  StateBox *cyc(); // a+

  State_Node *getStart();
  vector<State_Node *> getEnds();
};

StateBox *box();
StateBox *box(Condition *condition);
StateBox *box(long single);
StateBox *box(string str);
StateBox *box(long start, long end); // range

StateBox *con(string str);
StateBox *neg(string str);
StateBox *star(StateBox *item);

/**
 * build a fsm from a state map
 */
FSM *fsm(State_Node *node);
FSM *fsm(StateBox *StateBox);

// some common state
StateBox *hexDigit();
StateBox *digit();
StateBox *unicode();
StateBox *jsonString();
StateBox *jsonNumber();

// TODO clear function

} // namespace sfsm
