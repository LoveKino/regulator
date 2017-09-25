#include "commonTokenReg.h"

namespace sfsm {
const string CommonTokenReg::jsonStringExpStr = "\"((^[\"\\])|(\\([\"\\/bfnrt]|(u[0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f]))))*\"";
const string CommonTokenReg::jsonNumberExpStr = "";

RegularExp CommonTokenReg::jsonStringExp(CommonTokenReg::jsonStringExpStr);
RegularExp CommonTokenReg::jsonNumberExp(CommonTokenReg::jsonStringExpStr);
} // namespace sfsm
