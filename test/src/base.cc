#include "fsm.h"
#include <assert.h>
#include <iostream>

using namespace std;

void test(sfsm::FSM *m, string text) {
  string::iterator it;

  unsigned long index = 0;
  for (it = text.begin(); it != text.end(); ++it) {
    sfsm::TRANSITION_RESULT_TYPES t = m->transit(*it);
    cout << text << ":" << *it << " " << t << endl;
    ;
    if (index == text.size() - 1) {
      assert(t == sfsm::MATCH);
    } else {
      assert(t == sfsm::WAIT);
    }
    index++;
  }
}

void testQuit(sfsm::FSM *m, string text) {
  string::iterator it;

  unsigned long index = 0;
  for (it = text.begin(); it != text.end(); ++it) {
    sfsm::TRANSITION_RESULT_TYPES t = m->transit(*it);
    cout << text << ":" << *it << " " << t << endl;
    ;
    if (index == text.size() - 1) {
      assert(t == sfsm::QUIT);
    }
    index++;
  }
}

// ab
void test1() {
  sfsm::FSM *m = sfsm::fsm(sfsm::box("ab"));
  test(m, "ab");
}

// [ab]c
void test2() {
  test(sfsm::fsm(sfsm::box("a")->col("b")->row("c")), "bc");
  test(sfsm::fsm(sfsm::box("a")->col("b")->row("c")), "ac");
}

// abc|ade
void test3() {
  test(sfsm::fsm(sfsm::box("abc")->col("ade")), "abc");
  test(sfsm::fsm(sfsm::box("abc")->col("ade")), "ade");
}

int main() {
  test1();
  test2();
  test3();
  return 0;
}
