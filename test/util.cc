#include "./util.h"
using namespace sfsm;
using namespace std;

// TODO input-output matrix (equal, not equal)
namespace sfsm_test {
void displayParser(vector<string> regs) {
  Parser parser;
  for (auto it = regs.begin(); it != regs.end(); ++it) {
    ThompsonNFA tnfa = parser.parse(*it);

    cout << "regular expression is " << *it << endl;
    tnfa.getNFA().toDFA(tnfa.getStart()).first.display();
  }
}

void testRegMatch(string reg, vector<string> texts) {
  RegularExp exp(reg);
  for (auto it = texts.begin(); it != texts.end(); ++it) {
    assert(exp.test(*it));
  }
}

void testRegNotMatch(string reg, vector<string> texts) {
  RegularExp exp(reg);
  for (auto it = texts.begin(); it != texts.end(); ++it) {
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
