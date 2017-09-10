#pragma once

#include <iostream>
#include <vector>

using namespace std;

namespace sfsm {

class Condition {
public:
  // match function
  virtual bool match(long sign) = 0;
};

// equal value
class ValueCondition : public Condition {
private:
  long value;

public:
  ValueCondition(long value);
  bool match(long sign);
};

// range
class RangeCondition : public Condition {
private:
  long start;
  long end;

public:
  RangeCondition(long start, long end);
  bool match(long sign);
};

// Not

class NotCondition : public Condition {
private:
  long value;

public:
  NotCondition(long value);
  bool match(long sign);
};

// Or
class OrCondition : public Condition {
private:
  vector<Condition *> conditionList;

public:
  OrCondition(vector<Condition *> conditionList);
  bool match(long sign);
};

// And
class AndCondition : public Condition {
private:
  vector<Condition *> conditionList;

public:
  AndCondition(vector<Condition *> conditionList);
  bool match(long sign);
};

} // namespace sfsm
