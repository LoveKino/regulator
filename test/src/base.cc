#include "./util.h"
#include "unit.h"
#include <iostream>

using namespace sfsm;
using namespace std;

void testDisplayDFA() {
  cout << "[display dfa]" << endl;
  DFA dfa;
  dfa.addTransition(0, 'a', 1);
  dfa.addTransition(0, 'b', 2);
  dfa.addTransition(1, 'c', 3);

  dfa.display();
}

void testEqualDFA() {
  cout << "[equal dfa]" << endl;
  DFA dfa1;
  dfa1.addTransition(0, 'a', 1);
  dfa1.addTransition(0, 'b', 2);
  dfa1.addTransition(1, 'c', 3);
  dfa1.addTransition(1, 'd', 4);

  DFA dfa2;
  dfa2.addTransition(0, 'a', 1);
  dfa2.addTransition(0, 'b', 2);
  dfa2.addTransition(1, 'c', 3);
  dfa2.addTransition(1, 'd', 4);

  DFA dfa3;
  dfa3.addTransition(0, 'a', 1);
  dfa3.addTransition(0, 'b', 2);
  dfa3.addTransition(1, 'c', 3);
  dfa3.addTransition(1, 'e', 4);

  assert(dfa1 == dfa2);
  assert(dfa1 != dfa3);
}

void testNFAEpsilonClosure() {
  cout << "[nfa epsilonClosure]" << endl;
  NFA nfa;

  nfa.addTransition(0, 'a', 1);
  nfa.addTransition(1, 'b', 2);
  nfa.addEpsilonTransition(1, 3);
  nfa.addTransition(3, 'c', 4);

  // DFA dfa = nfa.toDFA();
  NFA::NFA_State_Set set0;
  set0.insert(0);
  nfa.displayNFA_State_set(nfa.epsilonClosure(set0));

  NFA::NFA_State_Set set1;
  set1.insert(1);
  nfa.displayNFA_State_set(nfa.epsilonClosure(set1));

  NFA::NFA_State_Set set2;
  set2.insert(2);
  nfa.displayNFA_State_set(nfa.epsilonClosure(set2));

  // dfa.display();
}

void testNFAToDFA() {
  cout << "[nfa todfa]" << endl;

  // empty
  NFA nfa1;
  DFA tar1;
  cout << "nfa1 - dfa1" << endl;
  assert(nfa1.toDFA(0).first == tar1);

  // NFA as DFA
  NFA nfa2;
  nfa2.addTransition(0, 'a', 1);
  nfa2.addTransition(0, 'b', 2);
  nfa2.addTransition(1, 'c', 3);
  nfa2.addTransition(2, 'd', 4);
  cout << "nfa2 - dfa2" << endl;
  nfa2.toDFA(0).first.display();

  // NFA contains sets

  NFA nfa3;
  nfa3.addTransition(0, 'a', 1);
  nfa3.addTransition(0, 'b', 2);
  nfa3.addTransition(1, 'c', 3);
  nfa3.addTransition(2, 'd', 4);
  nfa3.addTransition(2, 'd', 3);

  cout << "nfa3 - dfa3: contains sets" << endl;
  nfa3.toDFA(0).first.display();

  // NFA contains epsilon

  NFA nfa4;
  nfa4.addTransition(0, 'a', 1);
  nfa4.addTransition(0, 'a', 2);
  nfa4.addTransition(1, 'b', 3);
  nfa4.addEpsilonTransition(1, 4);
  nfa4.addTransition(4, 'c', 5);
  nfa4.addTransition(4, 'd', 6);

  cout << "nfa4 - dfa4: contains epsilon" << endl;
  nfa4.toDFA(0).first.display();

  // dfa.display();
}

void testThompsonConstruct() {
  cout << "[Thompson construction]" << endl;
  ThompsonConstruction tc;

  auto tnfa = tc.concatExpression(
      tc.symbol('a'), tc.unionExpression(tc.symbol('b'), tc.symbol('c')));

  tnfa.getNFA().display();
  tnfa.getNFA().toDFA(tnfa.getStart()).first.display();

  cout << "test star-------------" << endl;

  auto tnfa2 = tc.star(tc.concatExpression(tc.symbol('a'), tc.symbol('b')));
  tnfa2.getNFA().display();
  cout << "to dfa" << endl;
  cout << tnfa2.getStart() << "," << tnfa2.getEnd() << endl;
  tnfa2.getNFA().toDFA(tnfa2.getStart()).first.display();
}

void testRegTest() {
  cout << "regular test" << endl;
  sfsm_test::testRegMatrix(
      vector<sfsm_test::MatchPair>{
          // match case start
          {"a", {"a"}},
          {"a?", {"", "a"}},
          {"ca?b", {"cb", "cab"}},
          {"ab", {"ab"}},
          {"\\|", {"|"}},
          {"\\\\", {"\\"}},
          {"a|b", {"a", "b"}},
          {"a(b|c)d", {"abd", "acd"}},
          {"a*", {"", "a", "aa", "aaa"}},
          {"ba*", {"b", "ba", "baa"}},
          {"ba*c", {"bc", "bac"}},
          {"b(^[ab])*c", {"bc", "bfc"}},
          {"[0-4]", {"0", "1", "2", "3", "4"}},
          {"a[0-9](b|c)", {"a2b", "a3c"}},
          {"^a", {"b", "1", "c", "@", "/"}},
          {"c^a(e|f)", {"cce", "c1f"}},
          {"^[0-4]", {"5", "7"}},
          {"[aef]", {"a", "e", "f"}},
          {"[0-9a-f]", {"0", "4", "9", "a", "e", "f"}},
          {"^[\"\\\\]", {"h"}},
          {CommonTokenReg::jsonStringExpStr,
           {"\"\"", "\"hello world!\"", "\"1234\"",
            "\"one line \\n next line\\\"\"", "\"\\n \\r \\b \\u1234\""}},
          {CommonTokenReg::jsonNumberExpStr,
           {"1", "0", "123", "0.12", "0.467e10", "-100.23"}}
          // match case end
      },

      vector<sfsm_test::MatchPair>{
          // not match case start
          {"a", {"b", "c"}},
          {"a?", {"b", "aa"}},
          {"ca?b", {"c", "b", "ca"}},
          {"ab", {"a", "b", "abc"}},
          {"a|b", {"ab"}},
          {"a(b|c)d", {"abcd", "ad"}},
          {"a*", {"aba", "b"}},
          {"ba*", {"bb"}},
          {"[0-4]", {"5", "a", "r"}},
          {"a[0-9](b|c)", {"asb", "a3d"}},
          {"^a", {"a"}},
          {"c^a(e|f)", {"cae", "caf"}},
          {"^[0-4]", {"0", "1", "2", "3", "4"}},
          {"[aef]", {"b", "7", "k"}},
          {"[0-9a-f]", {"A", "B", "@"}},
          {CommonTokenReg::jsonStringExpStr, {"\"\\u12\"", "\"\\\"", "\"\\a\""}}
          // not match case end
      });
}

void testParser() {
  cout << "[Parser]" << endl;
  sfsm_test::displayParser(
      vector<string>{"ab", "abc", "ab|c", "a(b|c)", "(a(b|c)|ef)d", "a*"});
}

int main() {
  auto testGroup = unit_test::group("index", vector<unit_test::UnitCase> {
    unit_test::test("displayDFA", [&]() {
      testDisplayDFA();
    }),

    unit_test::test("testNFAEpsilonClosure", [&]() {
      testNFAEpsilonClosure();
    }),

    unit_test::test("testEqualDFA", [&]() {
      testEqualDFA();
    }),

    unit_test::test("testNFAToDFA", [&]() {
      testNFAToDFA();
    }),

    unit_test::test("testThompsonConstruct", [&]() {
      testThompsonConstruct();
    }),

    unit_test::test("testParser", [&]() {
      testParser();
    }),

    unit_test::test("testRegTest", [&]() {
      testRegTest();
    }),
  });

  testGroup.run();

  return 0;
}
