#include <iostream>

template<class... Args>
void fwd(Args&&... args) {
  // {{{
  int tmp[] __attribute__((unused))= {0, (std::cout << args << std::endl, 1)...};
  // }}}
}

int main() {
  fwd(1, "J'aime les moulinettes à fromage", false);
  return 0;
}
// vim: foldmethod=marker
