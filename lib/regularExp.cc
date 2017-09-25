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

unsigned int RegularExp::getStartState() {
    return 0;
}

bool RegularExp::test(string tar) {
  unsigned int curState = this->getStartState();

  for (auto it = tar.begin(); it != tar.end(); ++it) {
    int targetState = this->dfa.transit(curState, *it);
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

int RegularExp::transit(unsigned int state, char letter) {
  return this->dfa.transit(state, letter);
}

bool RegularExp::isEndState(int state) {
  return this->ends.find(state) != this->ends.end();
}

bool RegularExp::isErrorState(int state) { return state == -1; }

} // namespace sfsm
