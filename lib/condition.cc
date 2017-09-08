#include "condition.h"
#include <iostream>

using namespace std;

namespace sfsm {

// StringCondition
StringCondition::StringCondition(string value) { this->value = value; }

bool StringCondition::match(void *sign) {
  string *v = static_cast<string *>(sign);
  return !v->compare(this->value);
}

// OrCondition
OrCondition::OrCondition(vector<Condition *> *conditionList) {
  this->conditionList = conditionList;
}

bool OrCondition::match(void *sign) {
  for (vector<Condition *>::iterator it = this->conditionList->begin();
       it != this->conditionList->end(); ++it) {
    if ((*it)->match(sign)) {
      return true;
    }
  }

  return false;
}

// AndConcition
AndCondition::AndCondition(vector<Condition *> *conditionList) {
  this->conditionList = conditionList;
}

bool AndCondition::match(void *sign) {
  for (vector<Condition *>::iterator it = this->conditionList->begin();
       it != this->conditionList->end(); ++it) {
    if (!(*it)->match(sign)) {
      return false;
    }
  }

  return true;
}

} // namespace sfsm
