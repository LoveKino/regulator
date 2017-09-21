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

char Parser::OR_OP = '|';
char Parser::LEFT_BRACKET = '(';
char Parser::RIGHT_BRACKET = ')';
char Parser::VIRTUAL_CAT_OP = '1'; // virtual operation

bool Parser::isNormalLetter(char letter) {
  if (letter == OR_OP)
    return false;
  if (letter == LEFT_BRACKET)
    return false;
  if (letter == RIGHT_BRACKET)
    return false;
  return true;
}

void Parser::runUnionOp(vector<ThompsonNFA> &valueStack) {
  // TODO Exception
  auto c1 = valueStack.back();
  valueStack.pop_back();
  auto c2 = valueStack.back();
  valueStack.pop_back();
  valueStack.push_back(tc.unionExpression(c2, c1));
}

void Parser::runConcatOp(vector<ThompsonNFA> &valueStack) {
  auto c1 = valueStack.back();
  valueStack.pop_back();
  auto c2 = valueStack.back();
  valueStack.pop_back();
  valueStack.push_back(tc.concatExpression(c2, c1));
}

bool Parser::isNextConcated(char currentLetter, char nextLetter) {
  cout << currentLetter << ",      " << nextLetter << endl;
  if (nextLetter == OR_OP)
    return false;
  if (nextLetter == RIGHT_BRACKET) // a )
    return false;
  if (currentLetter == LEFT_BRACKET)
    return false;
  if (currentLetter == OR_OP)
    return false;
  return true;
};

bool Parser::reduceOpsStack(vector<ThompsonNFA> &valueStack,
                            vector<char> &ops) {
  while (ops.size()) {
    auto top = ops.back();
    if (top == OR_OP) {
      ops.pop_back();
      this->runUnionOp(valueStack);
    } else if (top == VIRTUAL_CAT_OP) {
      ops.pop_back();
      this->runConcatOp(valueStack);
    } else if (top == LEFT_BRACKET) { // touch the bottom
      return true;
    }
  }

  return false;
};

// TODO error situations
// infix situation
// Shunting-yard algorithm
// https://en.wikipedia.org/wiki/Shunting-yard_algorithm
ThompsonNFA Parser::parse(string regExp) {
  vector<ThompsonNFA> valueStack;
  vector<char> ops;

  int index = 0;
  int regExpSize = regExp.size();

  for (auto it = regExp.begin(); it != regExp.end(); ++it, ++index) {
    char letter = *it;

    if (letter == OR_OP) {
      ops.push_back(letter);
    } else if (letter == LEFT_BRACKET) {
      ops.push_back(letter);
    } else if (letter == RIGHT_BRACKET) {
      // pop ops until left bracket
      if (!this->reduceOpsStack(valueStack, ops)) {
        throw runtime_error("bracket '()' does not close correctly.");
      } else {
        ops.pop_back(); // pop LEFT_BRACKET
        // continue to reduce until meet LEFT_BRACKET
        this->reduceOpsStack(valueStack, ops);
      }
    } else {
      valueStack.push_back(tc.symbol(string(1, letter)));
      this->reduceOpsStack(valueStack, ops);
    }

    if (index < regExpSize - 1) {
      char nextLetter = *(next(it, 1));
      if (this->isNextConcated(letter, nextLetter)) {
        ops.push_back(VIRTUAL_CAT_OP);
      }
    }
  }

  return valueStack.back();
}
} // namespace sfsm
