#include <iostream>
#include <tuple>

struct foo {
  int field0_;
  float field1_;
};

// {{{
bool operator<(foo const& self, foo const& other) {
  return std::tie(self.field0_, self.field1_) < std::tie(other.field0_, other.field1_);
}
// }}}

int main() {
  foo f0{42, 4.2}, f1{42, 2.4};
  std::cout << (f0 < f1) << std::endl;
};
// vim: foldmethod=marker
