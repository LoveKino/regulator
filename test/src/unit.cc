#include "unit.h"

using namespace std;

namespace unit_test {
  void UnitCase::run() {
    this -> caseFun();

    cout << "\033[1;32m✓ [" << this->caseName << "]\033[0m" << endl;
  }

  void UnitGroup::run() {
    cout << "\033[1;35mgroup [" << this->groupName << "]\033[0m" << endl;
    for (auto unitCaseP =  this -> unitCases.begin(); unitCaseP != this -> unitCases.end(); ++ unitCaseP) {
        unitCaseP -> run();
    }
  }

  UnitCase test(string name, function<void()> fun) {
    return UnitCase(name, fun);
  }

  UnitGroup group(string name, vector<UnitCase> cases) {
    return UnitGroup(name, cases);
  }
}
