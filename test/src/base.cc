#include "../../lib/dfa.h"
#include "../../lib/nfa.h"
#include "../../lib/parser.h"
#include "assert.h"
#include <iostream>

using namespace sfsm;
using namespace std;

void testDisplayDFA() {
  cout << "[display dfa]" << endl;
  DFA dfa;
  dfa.addTransition(0, "a", 1);
  dfa.addTransition(0, "b", 2);
  dfa.addTransition(1, "c", 3);

  dfa.display();
}

void testEqualDFA() {
  cout << "[equal dfa]" << endl;
  DFA dfa1;
  dfa1.addTransition(0, "a", 1);
  dfa1.addTransition(0, "b", 2);
  dfa1.addTransition(1, "c", 3);
  dfa1.addTransition(1, "d", 4);

  DFA dfa2;
  dfa2.addTransition(0, "a", 1);
  dfa2.addTransition(0, "b", 2);
  dfa2.addTransition(1, "c", 3);
  dfa2.addTransition(1, "d", 4);

  DFA dfa3;
  dfa3.addTransition(0, "a", 1);
  dfa3.addTransition(0, "b", 2);
  dfa3.addTransition(1, "c", 3);
  dfa3.addTransition(1, "e", 4);

  assert(dfa1 == dfa2);
  assert(dfa1 != dfa3);
}

void testNFAEpsilonClosure() {
  cout << "[nfa epsilonClosure]" << endl;
  NFA nfa;

  nfa.addTransition(0, "a", 1);
  nfa.addTransition(1, "b", 2);
  nfa.addEpsilonTransition(1, 3);
  nfa.addTransition(3, "c", 4);

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
  assert(nfa1.toDFA(0) == tar1);

  // NFA as DFA
  NFA nfa2;
  nfa2.addTransition(0, "a", 1);
  nfa2.addTransition(0, "b", 2);
  nfa2.addTransition(1, "c", 3);
  nfa2.addTransition(2, "d", 4);
  nfa2.toDFA(0).display();

  // NFA contains sets

  NFA nfa3;
  nfa3.addTransition(0, "a", 1);
  nfa3.addTransition(0, "b", 2);
  nfa3.addTransition(1, "c", 3);
  nfa3.addTransition(2, "d", 4);
  nfa3.addTransition(2, "d", 3);

  nfa3.toDFA(0).display();

  // NFA contains epsilon

  NFA nfa4;
  nfa4.addTransition(0, "a", 1);
  nfa4.addTransition(0, "a", 2);
  nfa4.addTransition(1, "b", 3);
  nfa4.addEpsilonTransition(1, 4);
  nfa4.addTransition(4, "c", 5);
  nfa4.addTransition(4, "d", 6);

  nfa4.toDFA(0).display();

  // dfa.display();
}

void testThompsonConstruct() {
  cout << "[Thompson construction]" << endl;
  ThompsonConstruction tc;

  auto tnfa = tc.concatExpression(
      tc.symbol("a"), tc.unionExpression(tc.symbol("b"), tc.symbol("c")));

  tnfa.getNFA().display();
  tnfa.getNFA().toDFA(tnfa.getStart()).display();

  cout << "test star-------------" << endl;

  auto tnfa2 = tc.star(tc.concatExpression(tc.symbol("a"), tc.symbol("b")));
  tnfa2.getNFA().display();
  cout << "to dfa" << endl;
  cout << tnfa2.getStart() << "," << tnfa2.getEnd() << endl;
  tnfa2.getNFA().toDFA(tnfa2.getStart()).display();
}

void testParser() {
  cout << "[Parser]" << endl;
  Parser parser;

  ThompsonNFA tnfa1 = parser.parse("ab");
  tnfa1.getNFA().toDFA(tnfa1.getStart()).display();

  ThompsonNFA tnfa2 = parser.parse("abc");
  tnfa2.getNFA().toDFA(tnfa2.getStart()).display();

  ThompsonNFA tnfa3 = parser.parse("ab|c");
  tnfa3.getNFA().toDFA(tnfa3.getStart()).display();

  // test bracket
  ThompsonNFA tnfa4 = parser.parse("a(b|c)");
  tnfa4.getNFA().toDFA(tnfa4.getStart()).display();

  ThompsonNFA tnfa5 = parser.parse("(a(b|c)|ef)d");
  tnfa5.getNFA().toDFA(tnfa5.getStart()).display();
}

int main() {
  testDisplayDFA();
  testEqualDFA();
  testNFAEpsilonClosure();
  testNFAToDFA();
  testThompsonConstruct();
  testParser();
  return 0;
}
