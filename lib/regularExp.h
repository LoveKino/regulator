#pragma once

#include "dfa.h"
#include "unordered_set"
#include "parser.h"

using namespace std;

namespace sfsm {
class RegularExp {
private:
  DFA dfa;
  unordered_set<unsigned int> ends;

public:
  RegularExp(string regExp);

  bool test(string tar);
};
} // namespace sfsm
