#include <iostream>

#if 0
struct Foo {
  Foo() { std::cout << "Foo" << std::endl; }
  void doit() { std::cout << "Foo::doit" << std::endl; }
};
struct Bar {
  Bar() { foo.doit(); }
  static Foo foo;
};

Bar f;
Foo Bar::foo;
#else
struct Foo {
  Foo() { std::cout << "Foo" << std::endl; }
  void doit() { std::cout << "Foo::doit" << std::endl; }
};
struct Bar {
  Bar() { foo().doit(); }
  Foo& foo() {
    static Foo foo;
    return foo;
  }
};

Bar f;
#endif

int main() {
  return 0;
}
