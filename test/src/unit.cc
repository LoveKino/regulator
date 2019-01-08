#include "unit.h"

using namespace std;

namespace unit_test {
  void UnitCase::run() {
    cout << "run case [" << this->caseName << "]" << endl;
  }

  void UnitGroup::run() {
    cout << "run group [" << this->groupName << "]" << endl;
  }

  UnitCase test(string name, function<void()> fun) {
    return UnitCase(name, fun);
  }

  UnitGroup group(string name, vector<UnitCase> cases) {
    return UnitGroup(name, cases);
  }
}
