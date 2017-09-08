#include "fsm.h"
#include <assert.h>
#include <iostream>

using namespace std;

// ab
void test1() {
  sfsm::State_Node *s1 = new sfsm::State_Node();
  sfsm::State_Node *s2 = new sfsm::State_Node();
  sfsm::State_Node *s3 = new sfsm::State_Node(sfsm::ACCEPT);

  s1->addTransition(new sfsm::StringCondition("a"), s2);
  s2->addTransition(new sfsm::StringCondition("b"), s3);

  sfsm::FSM *m = sfsm::fsm(s1);

  assert(m->transit("a") == sfsm::WAIT);
  assert(m->transit("b") == sfsm::MATCH);
  assert(m->transit("c") == sfsm::QUIT);
  assert(m->transit("d") == sfsm::QUIT);
}

// [ab]c
void test2() {
  sfsm::StringCondition *c1 = new sfsm::StringCondition("a");
  sfsm::StringCondition *c2 = new sfsm::StringCondition("b");
  sfsm::StringCondition *c3 = new sfsm::StringCondition("c");
  vector<sfsm::Condition *> list;
  list.push_back(c1);
  list.push_back(c2);
  sfsm::OrCondition *or1 = new sfsm::OrCondition(&list);

  // test1
  sfsm::State_Node *s1 = new sfsm::State_Node();
  sfsm::State_Node *s2 = new sfsm::State_Node();
  sfsm::State_Node *s3 = new sfsm::State_Node(sfsm::ACCEPT);

  s1->addTransition(or1, s2);
  s2->addTransition(c3, s3);

  sfsm::FSM *m = sfsm::fsm(s1);

  assert(m->transit("a") == sfsm::WAIT);
  assert(m->transit("c") == sfsm::MATCH);

  // test2
  sfsm::State_Node *s4 = new sfsm::State_Node();
  sfsm::State_Node *s5 = new sfsm::State_Node();
  sfsm::State_Node *s6 = new sfsm::State_Node(sfsm::ACCEPT);

  s4->addTransition(or1, s5);
  s5->addTransition(c3, s6);

  sfsm::FSM *m2 = sfsm::fsm(s4);

  assert(m2->transit("b") == sfsm::WAIT);
  assert(m2->transit("c") == sfsm::MATCH);
}

int main() {
  test1();
  test2();
  return 0;
}
