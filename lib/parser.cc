#include "parser.h"

namespace sfsm {

ThompsonNFA::ThompsonNFA(NFA nfa, unsigned int start, unsigned int end) {
  this->nfa = nfa;
  this->start = start;
  this->end = end;
}

NFA &ThompsonNFA::getNFA() { return this->nfa; }

unsigned int ThompsonNFA::getStart() { return this->start; }

unsigned int ThompsonNFA::getEnd() { return this->end; }

ThompsonConstruction::ThompsonConstruction() { this->stateCount = 0; }

unsigned int ThompsonConstruction::getNewState() {
  auto v = this->stateCount;

  this->stateCount++;

  return v;
}

ThompsonNFA ThompsonConstruction::emptyExpression() {
  NFA nfa;
  auto state = getNewState();
  return ThompsonNFA(nfa, state, state);
}

ThompsonNFA ThompsonConstruction::symbol(string letter) {
  NFA nfa;
  auto start = getNewState();
  auto end = getNewState();

  nfa.addTransition(start, letter, end);

  return ThompsonNFA(nfa, start, end);
}

void ThompsonConstruction::mergeNFA(NFA &nfa1, NFA &n2) {
  auto n2Graph = n2.getTransitionGraph();
  auto n2Eps = n2.getEpsilonTransitions();
  // merge exist nfas
  nfa1.getTransitionGraph().insert(n2Graph.begin(), n2Graph.end());
  nfa1.getEpsilonTransitions().insert(n2Eps.begin(), n2Eps.end());
}

ThompsonNFA ThompsonConstruction::unionExpression(ThompsonNFA n1,
                                                  ThompsonNFA n2) {
  NFA nfa = n1.getNFA();
  auto start = getNewState();
  auto end = getNewState();

  this->mergeNFA(nfa, n2.getNFA());

  nfa.addEpsilonTransition(start, n1.getStart());
  nfa.addEpsilonTransition(start, n2.getStart());

  nfa.addEpsilonTransition(n1.getEnd(), end);
  nfa.addEpsilonTransition(n2.getEnd(), end);

  return ThompsonNFA(nfa, start, end);
}

ThompsonNFA ThompsonConstruction::concatExpression(ThompsonNFA n1,
                                                   ThompsonNFA n2) {
  NFA nfa = n1.getNFA();
  this->mergeNFA(nfa, n2.getNFA());
  nfa.addEpsilonTransition(n1.getEnd(), n2.getStart());

  return ThompsonNFA(nfa, n1.getStart(), n2.getEnd());
}

ThompsonNFA ThompsonConstruction::star(ThompsonNFA n) {
  auto start = this->getNewState();
  auto end = this->getNewState();
  auto nfa = n.getNFA();

  nfa.addEpsilonTransition(start, n.getStart());
  nfa.addEpsilonTransition(start, end);
  nfa.addEpsilonTransition(n.getEnd(), n.getStart());
  nfa.addEpsilonTransition(n.getEnd(), end);

  return ThompsonNFA(nfa, start, end);
}

const char Parser::OR_OP = '|';
const char Parser::STAR_OP = '*';
const char Parser::VIRTUAL_CAT_OP = '1'; // virtual operation
const char Parser::LEFT_BRACKET = '(';
const char Parser::RIGHT_BRACKET = ')';
unordered_map<char, int> Parser::OP_PRIORITY_MAP{{'|', 1}, {'*', 4}, {'1', 1}};

void Parser::runUnionOp(vector<ThompsonNFA> &valueStack) {
  // TODO Exception
  auto c1 = valueStack.back();
  valueStack.pop_back();
  auto c2 = valueStack.back();
  valueStack.pop_back();
  valueStack.push_back(tc.unionExpression(c2, c1));
}

void Parser::runConcatOp(vector<ThompsonNFA> &valueStack) {
  // TODO Exception
  auto c1 = valueStack.back();
  valueStack.pop_back();
  auto c2 = valueStack.back();
  valueStack.pop_back();
  valueStack.push_back(tc.concatExpression(c2, c1));
}

void Parser::runStarOp(vector<ThompsonNFA> &valueStack) {
  // TODO Exception
  auto c = valueStack.back();
  valueStack.pop_back();
  valueStack.push_back(tc.star(c));
}

bool Parser::isNextConcated(char currentLetter, char nextLetter) {
  return !(nextLetter == STAR_OP ||         //...*
           nextLetter == OR_OP ||           // ...|
           nextLetter == RIGHT_BRACKET ||   // ...)
           currentLetter == LEFT_BRACKET || // (..
           currentLetter == OR_OP           // |...
  );
};

bool Parser::reduceOpsStack(vector<ThompsonNFA> &valueStack,
                            vector<char> &ops) {
  while (ops.size()) {
    auto top = ops.back();
    switch (top) {
    case OR_OP:
      ops.pop_back();
      this->runUnionOp(valueStack);
      break;
    case VIRTUAL_CAT_OP:
      ops.pop_back();
      this->runConcatOp(valueStack);
      break;
    case STAR_OP:
      ops.pop_back();
      this->runStarOp(valueStack);
      break;
    case LEFT_BRACKET: // touch the bottom
      return true;
    }
  }

  return false;
};

void Parser::pushOp(char opLetter, vector<ThompsonNFA> &valueStack,
                    vector<char> &ops) {

  // before push OP, compare the priority to reduce op stack.
  if (ops.size()) {
    auto top = ops.back();
    auto findedItem = OP_PRIORITY_MAP.find(top);
    if (findedItem != OP_PRIORITY_MAP.end()) {
      if (OP_PRIORITY_MAP[opLetter] <
          findedItem->second) { // op in stack has a bigger priority
        this->reduceOpsStack(valueStack, ops);
      }
    }
  }

  ops.push_back(opLetter);
}

// TODO error situations
// infix situation
// Shunting-yard algorithm
// https://en.wikipedia.org/wiki/Shunting-yard_algorithm
ThompsonNFA Parser::parse(string regExp) {
  int regExpSize = regExp.size();
  if (regExpSize == 0) {
    return tc.emptyExpression();
  }

  vector<ThompsonNFA> valueStack;
  vector<char> ops;

  int index = 0;

  for (auto it = regExp.begin(); it != regExp.end(); ++it, ++index) {
    char letter = *it;

    if (letter == OR_OP || letter == STAR_OP) {
      this->pushOp(letter, valueStack, ops);
    } else if (letter == LEFT_BRACKET) {
      ops.push_back(letter);
    } else if (letter == RIGHT_BRACKET) {
      // pop ops until meeting left bracket
      if (!this->reduceOpsStack(valueStack, ops)) {
        throw runtime_error("bracket '()' does not close correctly.");
      } else {
        ops.pop_back(); // pop LEFT_BRACKET
        // continue to reduce until meet LEFT_BRACKET
        this->reduceOpsStack(valueStack, ops);
      }
    } else {
      valueStack.push_back(tc.symbol(string(1, letter)));
    }

    if (index < regExpSize - 1) {
      char nextLetter = *(next(it, 1));
      if (this->isNextConcated(letter, nextLetter)) {
        ops.push_back(VIRTUAL_CAT_OP);
      }
    }
  }

  this->reduceOpsStack(valueStack, ops);

  return valueStack.back();
}

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
