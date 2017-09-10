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
  return this;
}

StateBox *StateBox::col(string str) { return this->col(new StateBox(str)); }

// connect the end to head
// TODO end match problem
StateBox *StateBox::cyc() {
  copyNodeTransitionsToList(this->ends, this->start);
  return this;
}

vector<State_Node *> StateBox::getEnds() { return this->ends; }

StateBox *box(Condition *condition) { return new StateBox(condition); }
StateBox *box(long single) { return new StateBox(single); }
StateBox *box(string str) { return new StateBox(str); }
StateBox *con(string str) {
  if (str.size() == 0) {
    throw runtime_error("string is empty.");
  }
  string::iterator it = str.begin();
  ++it;
  StateBox *start = box(*it);
  while (it != str.end()) {
    start->col(box(*it)); // col next char
    ++it;
  }

  return start;
}

StateBox *box(long start, long end) {
  return new StateBox(new RangeCondition(start, end));
}

StateBox *neg(string str) {
  vector<Condition *> list;
  string::iterator it;
  for (it = str.begin(); it != str.end(); ++it) {
    list.push_back(new ValueCondition(*it));
  }

  Condition *con = new NotCondition(new AndCondition(list));

  return new StateBox(con);
}

FSM *fsm(State_Node *node) { return new FSM(node); }

FSM *fsm(StateBox *box) {
  // mark ends nodes as ACCEPT nodes
  vector<State_Node *>::iterator it;
  for (it = box->getEnds().begin(); it != box->getEnds().end(); ++it) {
    (*it)->setType(ACCEPT);
  }

  return fsm(box->getStart());
}

StateBox *hexDigit() {
  return box('a', 'F')->col(box('0', '9'))->col(box('A', 'F'));
}

// \uD100
StateBox *unicode() {
  return box("\\u")
      ->row(hexDigit())
      ->row(hexDigit())
      ->row(hexDigit())
      ->row(hexDigit());
};

StateBox *jsonString() {
  StateBox *SpecialEscape = box("\\")->row("\"\\/bfnrt")->col(unicode());
  StateBox *rest = neg("\"\\");
  StateBox *middle = SpecialEscape->col(rest)->cyc();

  return box("\"")->row(middle)->row("\"")->
      // ""
      col("\"\"");
}

} // namespace sfsm
