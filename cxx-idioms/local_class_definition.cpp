// run nm -C on this
#include <iostream>

struct Global{
  Global() { std::cout << "Global" << std::endl;}
};

namespace {

struct Local{
  Local() { std::cout << "Local" << std::endl;}
};

}
int main() {
  Global g;
  Local l;
  return 0;
}
// vim: foldmethod=marker
