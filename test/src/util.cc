#include "./util.h"
using namespace sfsm;
using namespace std;

namespace sfsm_test {
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

typedef vector<string> StringList;
typedef pair<string, StringList> MatchPair;
void testRegMatrix(vector<MatchPair> matchMatrix,
                   vector<MatchPair> notMatchMatrix) {
  for (auto i = matchMatrix.begin(); i != matchMatrix.end(); ++i) {
    string reg = i->first;
    auto list = i->second;
    testRegMatch(reg, list);
  }

  for (auto j = notMatchMatrix.begin(); j != notMatchMatrix.end(); ++j) {
    string reg = j->first;
    auto list = j->second;
    testRegNotMatch(reg, list);
  }
}
}
