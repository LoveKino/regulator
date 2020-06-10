#include "./util.h"
#include "../deps/ccunit/lib/unit.h"
#include <iostream>

using namespace sfsm;
using namespace std;

int main() {
    unit_test::group("DFA & NFA", vector<unit_test::UnitCase> {
      unit_test::test("displayDFA", [&]() {
        DFA dfa;
        dfa.addTransition(0, 'a', 1);
        dfa.addTransition(0, 'b', 2);
        dfa.addTransition(1, 'c', 3);

        assert(dfa.transit(0, 'a') == 1);
        assert(dfa.transit(0, 'b') == 2);
        assert(dfa.transit(1, 'c') == 3);
        assert(dfa.transit(1, 'a') == -1);
      }),

      unit_test::test("testEqualDFA", [&]() {
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
      }),
    }).run();

    unit_test::group("Thompson", vector<unit_test::UnitCase> {
      unit_test::test("testThompsonConstruct", [&]() {
        ThompsonConstruction tc;

        auto tnfa = tc.concatExpression(
            tc.symbol('a'), tc.unionExpression(tc.symbol('b'), tc.symbol('c')));

        auto tnfa2 = tc.star(tc.concatExpression(tc.symbol('a'), tc.symbol('b')));
        assert(tnfa2.getStart() == 12);
        assert(tnfa2.getEnd() == 13);
      })
    }).run();

    unit_test::group("parser", vector<unit_test::UnitCase> { 
      unit_test::test("testRegTest", [&]() {
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
      }),
    }).run();

  return 0;
}
