#pragma once

#include <iostream>
#include <vector>

using namespace std;

namespace sfsm {

class Condition {
public:
  virtual bool match(void *sign) = 0;
};

// string
class StringCondition : public Condition {
private:
  string value;

public:
  StringCondition(string value);
  bool match(void *sign);
};

// Or
class OrCondition : public Condition {
private:
  vector<Condition *> *conditionList;

public:
  OrCondition(vector<Condition *> *conditionList);
  bool match(void *sign);
};

// And
class AndCondition : public Condition {
private:
  vector<Condition *> *conditionList;

public:
  AndCondition(vector<Condition *> *conditionList);
  bool match(void *sign);
};

} // namespace sfsm
