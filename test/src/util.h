#pragma once

#include "../../lib/commonTokenReg.h"
#include "../../lib/dfa.h"
#include "../../lib/nfa.h"
#include "../../lib/regularExp.h"
#include "assert.h"
#include <iostream>

using namespace sfsm;
using namespace std;

namespace sfsm_test {
void displayParser(vector<string> regs);

void testRegMatch(string reg, vector<string> texts);

void testRegNotMatch(string reg, vector<string> texts);

typedef vector<string> StringList;
typedef pair<string, StringList> MatchPair;
void testRegMatrix(vector<MatchPair> matchMatrix,
                   vector<MatchPair> notMatchMatrix);
};
