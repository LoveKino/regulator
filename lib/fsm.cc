#include "fsm.h"
#include <iostream>

using namespace std;

namespace sfsm {

vector<Condition *> getConditionsFromString(string str) {
  string::iterator it;
  vector<Condition *> list;
  for (it = str.begin(); it != str.end(); ++it) {
    list.push_back(new ValueCondition(*it));
  }

  return list;
}

void copyNodeTransitionsToList(vector<State_Node *> list, State_Node *n) {
  vector<State_Node *>::iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    (*it)->copyNodeTransitions(n);
  }
}

void StateBox::init(Condition *condition) {
  State_Node *start = new State_Node();
  State_Node *end = new State_Node();
  start->addTransition(condition, end);

  this->ends.push_back(end);
  this->start = start;
}

StateBox::StateBox(Condition *condition) { this->init(condition); }

StateBox::StateBox(long single) { this->init(new ValueCondition(single)); }

StateBox::StateBox(string str) {
  if (!str.size()) {
    throw runtime_error("string must not be empty when build a row of states.");
  }
  string::iterator it = str.begin();

  this->init(new ValueCondition(*it));
  ++it;

  while (it != str.end()) {
    StateBox *next = new StateBox(*it);
    this->row(next);
    ++it;
  }
}

State_Node *StateBox::getStart() { return this->start; }

StateBox *StateBox::row(StateBox *box) {
  State_Node *secondHead = box->start;

  copyNodeTransitionsToList(this->ends, secondHead);
  secondHead->clear(); // TODO

  this->ends.clear();
  this->ends.insert(this->ends.end(), box->ends.begin(), box->ends.end());

  return this;
}

StateBox *StateBox::row(string str) { return this->row(new StateBox(str)); }
StateBox *StateBox::row(Condition *c) { return this->row(new StateBox(c)); }
StateBox *StateBox::row(long single) { return this->row(new StateBox(single)); }

StateBox *StateBox::col(StateBox *box) {
  this->start->copyNodeTransitions(box->start); // merge first nodes
  this->ends.insert(this->ends.end(), box->ends.begin(),
                    box->ends.end()); // merge ends
  // TODO clear
  return this;
}

StateBox *StateBox::col(string str) { return this->col(new StateBox(str)); }

StateBox *box(Condition *condition) { return new StateBox(condition); }
StateBox *box(long single) { return new StateBox(single); }
StateBox *box(string str) { return new StateBox(str); }

FSM *fsm(State_Node *node) { return new FSM(node); }
FSM *fsm(StateBox *box) { return new FSM(box->getStart()); }

} // namespace sfsm
