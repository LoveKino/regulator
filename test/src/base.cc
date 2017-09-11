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

  test(sfsm::fsm(sfsm::box("o")->row(sfsm::box()->col("a"))), "o");
  test(sfsm::fsm(sfsm::box("o")->row(sfsm::star(sfsm::box("abc")))), "oabc");
  test(sfsm::fsm(sfsm::box("o")->row(sfsm::star(sfsm::box("abc")))), "oabcabc");
  test(sfsm::fsm(sfsm::box("o")->row(sfsm::star(sfsm::box("abc")))), "o");

  // json number
  test(sfsm::fsm(sfsm::jsonNumber()), "1");             // single
  test(sfsm::fsm(sfsm::jsonNumber()), "123");           // integer
  test(sfsm::fsm(sfsm::jsonNumber()), "-123");          // negative
  test(sfsm::fsm(sfsm::jsonNumber()), "22.890");        //
  test(sfsm::fsm(sfsm::jsonNumber()), "0.12");          //
  testQuit(sfsm::fsm(sfsm::jsonNumber()), "012");       //
  testQuit(sfsm::fsm(sfsm::jsonNumber()), "-48.5e10");  //
  testQuit(sfsm::fsm(sfsm::jsonNumber()), "-48.5e-10"); //

  // quit
  testQuit(sfsm::fsm(sfsm::box("abc")->col("ade")), "afe");
  testQuit(sfsm::fsm(sfsm::box("ab")), "abe");

  // border
  test(sfsm::fsm(sfsm::box()->row("ab")), "ab");
  test(sfsm::fsm(sfsm::box()->col("ab")->row("c")), "abc");
  test(sfsm::fsm(sfsm::box()->col("ab")->row("c")), "c");
  test(sfsm::fsm(sfsm::box()->col("a")->row("b")->row(sfsm::box())), "ab");
  return 0;
}
