#include "commonTokenReg.h"

namespace sfsm {
const string CommonTokenReg::jsonStringExpStr = "";
const string CommonTokenReg::jsonNumberExpStr = "";

RegularExp CommonTokenReg::jsonStringExp(CommonTokenReg::jsonStringExpStr);
RegularExp CommonTokenReg::jsonNumberExp(CommonTokenReg::jsonStringExpStr);
}
