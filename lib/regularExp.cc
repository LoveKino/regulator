#include "regularExp.h"

namespace sfsm {
RegularExp::RegularExp(string exp) {
  Parser parser;
  auto tnfa = parser.parse(exp);
  auto ret = tnfa.getNFA().toDFA(tnfa.getStart());
  unsigned int end = tnfa.getEnd();

  this->dfa = ret.first;
  auto stateMap = ret.second;

  for (auto it = stateMap.begin(); it != stateMap.end(); ++it) {
    unsigned int state = it->first;
    NFA::NFA_State_Set nfaStateSet = it->second;
    if (nfaStateSet.find(end) != nfaStateSet.end()) {
      this->ends.insert(state);
    }
  }
}

bool RegularExp::test(string tar) {
  unsigned int curState = 0;

  for (auto it = tar.begin(); it != tar.end(); ++it) {
    string letter = string(1, *it);
    int targetState = this->dfa.transit(curState, letter);
    if (targetState == -1) {
      return false;
    } else {
      curState = targetState;
    }
  }

  if (this->ends.find(curState) == this->ends.end()) { // not accept state
    return false;
  }

  return true;
}

} // namespace sfsm
