#include "fsm.h"
#include <iostream>

using namespace std;

namespace sfsm {

RowStates row(Condition *condition) { return row(condition, MIDDLE); };

RowStates row(Condition *condition, STATE_NODE_TYPES type) {
  vector<Condition *> list;
  list.push_back(condition);
  return row(list, type);
};

RowStates row(vector<Condition *> conditionList) {
  return row(conditionList, MIDDLE);
};

RowStates row(vector<Condition *> conditionList, STATE_NODE_TYPES type) {
  State_Node *start = new State_Node();
  State_Node *cur = start;
  vector<Condition *>::iterator it;
  for (it = conditionList.begin(); it != conditionList.end(); ++it) {
    State_Node *next = new State_Node();
    cur->addTransition((*it), next);
    cur = next;
  }

  cur->setType(type);
  return make_pair(start, cur);
}

RowStates row(string str) { return row(str, MIDDLE); };

RowStates row(string str, STATE_NODE_TYPES type) {
  State_Node *start = new State_Node();
  State_Node *cur = start;
  string::iterator it;
  for (it = str.begin(); it != str.end(); ++it) {
    State_Node *next = new State_Node();
    cur->addTransition(new ValueCondition(*it), next);
    cur = next;
  }

  cur->setType(type);
  return make_pair(start, cur);
}

RowStates connect(Condition *c1, Condition *c2) {
  State_Node *t1 = new State_Node();
  State_Node *t2 = new State_Node();
  State_Node *t3 = new State_Node();

  t1->addTransition(c1, t2);
  t2->addTransition(c2, t3);

  return make_pair(t1, t3);
}

RowStates connect(RowStates row1, Condition *c) {
  State_Node *frontHead = row1.first;
  State_Node *frontEnd = row1.second;
  State_Node *end = new State_Node();
  frontEnd->addTransition(c, end);
  return make_pair(frontHead, end);
}

RowStates connect(RowStates row1, Condition *c, RowStates row2) {
  State_Node *frontHead = row1.first;
  State_Node *frontEnd = row1.second;
  State_Node *backHead = row2.first;
  State_Node *backEnd = row2.second;

  frontEnd->addTransition(c, backHead);

  return make_pair(frontHead, backEnd);
}

FSM *fsm(State_Node *node) { return new FSM(node); }

FSM *fsm(RowStates rowState) { return new FSM(rowState.first); }

} // namespace sfsm
