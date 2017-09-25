#pragma once

#include "regularExp.h"

namespace sfsm {
class CommonTokenReg {
public:
  const static string jsonStringExpStr;
  const static string jsonNumberExpStr;
  static RegularExp jsonStringExp;
  static RegularExp jsonNumberExp;
};
} // namespace sfsm
