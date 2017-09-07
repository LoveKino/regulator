#include "fsm.h"
#include <iostream>

using namespace std;

int main() {
  sfsm::State_Node *s1 = new sfsm::State_Node();
  sfsm::State_Node *s2 = new sfsm::State_Node();
  sfsm::State_Node *s3 = new sfsm::State_Node(sfsm::ACCEPT);

  s1->addTransition("a", s2);
  s2->addTransition("b", s3);

  sfsm::FSM *m = sfsm::fsm(s1);

  cout << m->transit("a") << endl;
  cout << m->transit("b") << endl;
  cout << m->transit("c") << endl;

  return 0;
}
