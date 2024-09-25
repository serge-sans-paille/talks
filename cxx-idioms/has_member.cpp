#include <iostream>

template<class T>
class has_member {
  // {{{
  template<class C> static int has(typename C::member*);
  template<class C> static void has(...);
  public:
  static constexpr bool value = std::is_same<decltype(has<T>(nullptr)), int>::value;
  // }}}
};

struct With {
  using member = void;
};
struct WithOut {
};

int main() {
  std::cout << "With: " << has_member<With>::value << std::endl;
  std::cout << "WithOut: " << has_member<WithOut>::value << std::endl;
  return 0;
}

// vim: foldmethod=marker
