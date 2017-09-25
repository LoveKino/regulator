#include "commonTokenReg.h"

namespace sfsm {
const string CommonTokenReg::jsonStringExpStr = "\"(^[\"\\\\]|\\\\([\"\\\\/bfnrt]|u[0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f][0-9A-Fa-f]))*\"";
const string CommonTokenReg::jsonNumberExpStr = "-?(0|[1-9][0-9]*)(.[0-9][0-9]*)?([eE][\\+\\-]?[0-9][0-9]*)?";

RegularExp CommonTokenReg::jsonStringExp(CommonTokenReg::jsonStringExpStr);
RegularExp CommonTokenReg::jsonNumberExp(CommonTokenReg::jsonNumberExpStr);
} // namespace sfsm
