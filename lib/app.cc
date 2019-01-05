#include "app.h"
#include "regularExp.h"

namespace sfsm {
  bool test(string regExp, string tar) {
    RegularExp exp(regExp);
    return exp.test(tar);
  }
}
