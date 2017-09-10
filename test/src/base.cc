#include "../../lib/fsm.h"
#include <assert.h>
#include <iostream>

using namespace std;

void test(sfsm::FSM *m, string text) {
  cout << "[" << text << "]" << endl;
  string::iterator it;

  unsigned long index = 0;
  for (it = text.begin(); it != text.end(); ++it) {
    sfsm::TRANSITION_RESULT_TYPES t = m->transit(*it);
    cout << *it << " " << t << endl;
    if (index == text.size() - 1) {
      assert(t == sfsm::MATCH);
    } else {
      assert(t == sfsm::WAIT || t == sfsm::MATCH);
    }
    index++;
  }
}

void testQuit(sfsm::FSM *m, string text) {
  cout << "[" << text << "]" << endl;
  string::iterator it;

  unsigned long index = 0;
  for (it = text.begin(); it != text.end(); ++it) {
    sfsm::TRANSITION_RESULT_TYPES t = m->transit(*it);
    cout << *it << " " << t << endl;
    ;
    if (index == text.size() - 1) {
      assert(t == sfsm::QUIT);
    }
    index++;
  }
}

int main() {
  // normal
  test(sfsm::fsm(sfsm::box("ab")), "ab");
  test(sfsm::fsm(sfsm::box("a")->col("b")->row("c")), "bc");
  test(sfsm::fsm(sfsm::box("a")->col("b")->row("c")), "ac");
  test(sfsm::fsm(sfsm::box("abc")->col("ade")), "abc");
  test(sfsm::fsm(sfsm::box("abc")->col("ade")), "ade");
  test(sfsm::fsm(sfsm::box("ab")->cyc()), "ab");
  test(sfsm::fsm(sfsm::box("ab")->cyc()), "ababab");
  test(sfsm::fsm(sfsm::box("ab")->cyc()->row("c")), "ababc");

  // json string
  test(sfsm::fsm(sfsm::jsonString()), "\"\"");           // empty string
  test(sfsm::fsm(sfsm::jsonString()), "\"123Kjd_&%^\""); // normal
  test(sfsm::fsm(sfsm::jsonString()), "\"\\u1323\"");    // unicode
  test(sfsm::fsm(sfsm::jsonString()), "\"\\b\"");        // escape
  test(sfsm::fsm(sfsm::jsonString()), "\"\\n\"");        // escape
  test(sfsm::fsm(sfsm::jsonString()), "\"\\\"\"");       // escape
  test(sfsm::fsm(sfsm::jsonString()), "\"\\\\\"");       // escape

  // quit
  testQuit(sfsm::fsm(sfsm::box("abc")->col("ade")), "afe");
  testQuit(sfsm::fsm(sfsm::box("ab")), "abe");

  return 0;
}
