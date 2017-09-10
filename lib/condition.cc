#include "condition.h"
#include <iostream>

using namespace std;

namespace sfsm {

// ValueCondition
ValueCondition::ValueCondition(long value) { this->value = value; }

bool ValueCondition::match(long sign) { return sign == this->value; }

// RangeCondition
RangeCondition::RangeCondition(long start, long end) {
  this->start = start;
  this->end = end;
}

bool RangeCondition::match(long sign) {
  return sign >= this->start && sign <= this->end;
}

// NotCondition
NotCondition::NotCondition(long value) { this->value = value; }

bool NotCondition::match(long sign) { return this->value != sign; };

// OrCondition
OrCondition::OrCondition(vector<Condition *> conditionList) {
  this->conditionList = conditionList;
}

bool OrCondition::match(long sign) {
  for (vector<Condition *>::iterator it = this->conditionList.begin();
       it != this->conditionList.end(); ++it) {
    if ((*it)->match(sign)) {
      return true;
    }
  }

  return false;
}

// AndConcition
AndCondition::AndCondition(vector<Condition *> conditionList) {
  this->conditionList = conditionList;
}

bool AndCondition::match(long sign) {
  for (vector<Condition *>::iterator it = this->conditionList.begin();
       it != this->conditionList.end(); ++it) {
    if (!(*it)->match(sign)) {
      return false;
    }
  }

  return true;
}

} // namespace sfsm
