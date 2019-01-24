#include "unit.h"
#include "vector"
#include <chrono>

using namespace std;

namespace unit_test {
  void UnitCase::run() {
    this -> caseFun();
  }

  uint64_t getCurrentTime() {
    std::chrono::high_resolution_clock m_clock;
    return std::chrono::duration_cast<std::chrono::milliseconds> (m_clock.now().time_since_epoch()).count();
  }

  int BLACK = 30;
  int RED = 31;
  int GREEN = 32;
  int YELLOW = 33;
  int BLUE = 34;
  int MAGENTA = 35;
  int CYAN = 36;
  int LIGHT_GRAY = 37;

  string getTextWithColor(string text, int color) {
    return "\033[1;" + std::to_string(color) + "m" + text + "\033[0m";
  }

  void UnitGroup::run() {
    cout << getTextWithColor("Group[" + this->groupName + "]", CYAN) << endl;

    vector<CaseResult> results;

    for (auto unitCaseP =  this -> unitCases.begin(); unitCaseP != this -> unitCases.end(); ++ unitCaseP) {
        cout << getTextWithColor("  - Run case [" + unitCaseP->caseName + "]", CYAN) << endl;
        uint64_t startTime = getCurrentTime();
        try {
          unitCaseP -> run();
          uint64_t endTime = getCurrentTime();

          results.push_back(CaseResult(unitCaseP->caseName, true, "", startTime, endTime));
        } catch (const std::exception& e) {
          uint64_t endTime = getCurrentTime();
          results.push_back(CaseResult(unitCaseP->caseName, false, e.what(), startTime, endTime));
        }
    }

    // display summarise
    cout << getTextWithColor("Group[" + this->groupName + "]", CYAN) << endl;
    for(auto caseRetP = results.begin(); caseRetP != results.end(); ++caseRetP) {
        auto duration = caseRetP->endTime - caseRetP->startTime;

        if(caseRetP->pass) {
          cout << "  " << getTextWithColor("✓ [" + caseRetP->caseName + "] " + caseRetP->errMsg, GREEN) << "(" << (caseRetP->endTime - caseRetP->startTime) << "ms)" << endl;
        } else {
          cout << "  " << getTextWithColor("✘ [" + caseRetP->caseName + "] " + caseRetP->errMsg, RED) << "(" << (caseRetP->endTime - caseRetP->startTime) << "ms)" << endl;
        }
    }
  }

  UnitCase test(string name, function<void()> fun) {
    return UnitCase(name, fun);
  }

  UnitGroup group(string name, vector<UnitCase> cases) {
    return UnitGroup(name, cases);
  }
}
