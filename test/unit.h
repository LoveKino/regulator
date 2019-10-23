#pragma once

#include "vector"
#include <iostream>

using namespace std;

namespace unit_test {
class UnitCase {
private:
  function<void()> caseFun;
public:
  string caseName;
  UnitCase(string name, function<void()> fun): caseName(name), caseFun(fun) {}
  void run();
};

class CaseResult {
public:
  string caseName;
  bool pass;
  string errMsg;
  uint64_t startTime;
  uint64_t endTime;
  CaseResult(string name, bool p, string err, uint64_t st, uint64_t et): caseName(name), pass(p), errMsg(err), startTime(st), endTime(et) {}
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
