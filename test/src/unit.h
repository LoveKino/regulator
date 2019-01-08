#pragma once

#include "vector"
#include <iostream>

using namespace std;

namespace unit_test {
class UnitCase {
private:
  string caseName;
  function<void()> caseFun;
public:
  UnitCase(string name, function<void()> fun): caseName(name), caseFun(fun) {}
  void run();
};

class UnitGroup {
public:
  UnitGroup(string name, vector<UnitCase> cases): groupName(name), unitCases(cases) {}
  void run();
private:
  string groupName;
  vector<UnitCase> unitCases;
};

  UnitCase test(string name, function<void()> fun);
  UnitGroup group(string name, vector<UnitCase> cases);
}
