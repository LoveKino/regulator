#include "../lib/app.h"
#include <iostream>

using namespace sfsm;
using namespace std;

int main(int argc, char** argv) {
  if(argc != 3) {
    cout << "for test function, need 2 args, but got "<< argc << ". eg: regulator .* abc." << endl;
    return 1;
  }

  string regExp(argv[1]);
  string tar(argv[2]);
  if(sfsm::test(regExp, tar)) {
    cout << "[test success] '" << argv[2] << "' do match regular expression: '" << argv[1] << "'" << endl;
    return 0;
  } else {
    cout << "\033[1;31m[test fail] '" << argv[2] << "' do not match regular expression: '" << argv[1] << "'" << "\033[0m" << endl;
    return 1;
  }
}
