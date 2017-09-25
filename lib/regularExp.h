#pragma once

#include "dfa.h"
#include "parser.h"
#include "unordered_set"

using namespace std;

namespace sfsm {
class RegularExp {
private:
  DFA dfa;
  unordered_set<unsigned int> ends;

public:
  RegularExp(string regExp);

  bool test(string tar);

  int transit(unsigned int state, char letter);

  unsigned int getStartState();

  bool isEndState(int state);

  bool isErrorState(int state);
};

} // namespace sfsm
