#include "fsm.h"
#include <assert.h>
#include <iostream>

using namespace std;

void test1() {
  sfsm::State_Node *s1 = new sfsm::State_Node();
  sfsm::State_Node *s2 = new sfsm::State_Node();
  sfsm::State_Node *s3 = new sfsm::State_Node(sfsm::ACCEPT);

  s1->addTransition("a", s2);
  s2->addTransition("b", s3);

  sfsm::FSM *m = sfsm::fsm(s1);

  assert(m->transit("a") == sfsm::WAIT);
  assert(m->transit("b") == sfsm::MATCH);
  assert(m->transit("c") == sfsm::QUIT);
  assert(m->transit("d") == sfsm::QUIT);
}

int main() {
  test1();
  return 0;
}
