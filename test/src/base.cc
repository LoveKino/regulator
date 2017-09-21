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
  assert(nfa1.toDFA(0).first == tar1);

  // NFA as DFA
  NFA nfa2;
  nfa2.addTransition(0, "a", 1);
  nfa2.addTransition(0, "b", 2);
  nfa2.addTransition(1, "c", 3);
  nfa2.addTransition(2, "d", 4);
  nfa2.toDFA(0).first.display();

  // NFA contains sets

  NFA nfa3;
  nfa3.addTransition(0, "a", 1);
  nfa3.addTransition(0, "b", 2);
  nfa3.addTransition(1, "c", 3);
  nfa3.addTransition(2, "d", 4);
  nfa3.addTransition(2, "d", 3);

  nfa3.toDFA(0).first.display();

  // NFA contains epsilon

  NFA nfa4;
  nfa4.addTransition(0, "a", 1);
  nfa4.addTransition(0, "a", 2);
  nfa4.addTransition(1, "b", 3);
  nfa4.addEpsilonTransition(1, 4);
  nfa4.addTransition(4, "c", 5);
  nfa4.addTransition(4, "d", 6);

  nfa4.toDFA(0).first.display();

  // dfa.display();
}

void testThompsonConstruct() {
  cout << "[Thompson construction]" << endl;
  ThompsonConstruction tc;

  auto tnfa = tc.concatExpression(
      tc.symbol("a"), tc.unionExpression(tc.symbol("b"), tc.symbol("c")));

  tnfa.getNFA().display();
  tnfa.getNFA().toDFA(tnfa.getStart()).first.display();

  cout << "test star-------------" << endl;

  auto tnfa2 = tc.star(tc.concatExpression(tc.symbol("a"), tc.symbol("b")));
  tnfa2.getNFA().display();
  cout << "to dfa" << endl;
  cout << tnfa2.getStart() << "," << tnfa2.getEnd() << endl;
  tnfa2.getNFA().toDFA(tnfa2.getStart()).first.display();
}

void displayParser(vector<string> regs) {
  cout << "[display parse result]" << endl;
  Parser parser;
  for (auto it = regs.begin(); it != regs.end(); ++it) {
    ThompsonNFA tnfa = parser.parse(*it);

    cout << "regular expression is " << *it << endl;
    tnfa.getNFA().toDFA(tnfa.getStart()).first.display();
  }
}

void testRegMatch(string reg, vector<string> texts) {
  cout << "[test reg match]" << endl;
  RegularExp exp(reg);
  for (auto it = texts.begin(); it != texts.end(); ++it) {
    cout << "regular expression is " << reg << ", text is " << *it << endl;
    assert(exp.test(*it));
  }
}

void testRegNotMatch(string reg, vector<string> texts) {
  cout << "[test reg not match]" << endl;
  RegularExp exp(reg);
  for (auto it = texts.begin(); it != texts.end(); ++it) {
    cout << "regular expression is " << reg << ", text is " << *it << endl;
    assert(!exp.test(*it));
  }
}

void testRegTest() {
  cout << "regular test" << endl;
  testRegMatch("a", vector<string>{"a"});
  testRegNotMatch("a", vector<string>{"b", "c"});

  testRegMatch("ab", vector<string>{"ab"});
  testRegNotMatch("ab", vector<string>{"b", "a", "abc"});

  testRegMatch("a|b", vector<string>{"a", "b"});
  testRegNotMatch("a|b", vector<string>{"ab"});

  testRegMatch("a(b|c)d", vector<string>{"abd", "acd"});
  testRegNotMatch("a(b|c)d", vector<string>{"abcd", "ad"});
}

void testParser() {
  cout << "[Parser]" << endl;
  displayParser(vector<string>{"ab", "abc", "ab|c", "a(b|c)", "(a(b|c)|ef)d"});
}

int main() {
  testDisplayDFA();
  testEqualDFA();
  testNFAEpsilonClosure();
  testNFAToDFA();
  testThompsonConstruct();
  testParser();
  testRegTest();
  return 0;
}
