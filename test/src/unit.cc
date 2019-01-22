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

  void UnitGroup::run() {
    cout << "\033[1;35mgroup [" << this->groupName << "]\033[0m" << endl;

    vector<CaseResult> results;

    for (auto unitCaseP =  this -> unitCases.begin(); unitCaseP != this -> unitCases.end(); ++ unitCaseP) {
        cout << "  \033[1;35mrun case [" << unitCaseP->caseName << "]\033[0m" << endl;
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
    for(auto caseRetP = results.begin(); caseRetP != results.end(); ++caseRetP) {
        auto duration = caseRetP->endTime - caseRetP->startTime;

        if(caseRetP->pass) {
          cout << "  \033[1;32m✓ [" << caseRetP->caseName << "] (" << duration << "ms)\033[0m" << endl;
        } else {
          cout << "  \033[1;31m✘ [" << caseRetP->caseName << "] " << caseRetP->errMsg << "\033[0m" << endl;
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
