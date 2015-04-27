#include <iostream>

class Foo {
  int fieldA, fieldB, fieldC;
  public:
  Foo() = default;
  Foo& setA(int a) { fieldA = a; return *this; }
  Foo& setB(int b) { fieldB = b; return *this; }
  Foo& setC(int c) { fieldC = c; return *this; }
  void dump() const { std::cout << fieldA << ":" << fieldB << ":" << fieldC << std::endl; }
};

int main() {
  Foo f;
  f.setA(1)
   .setB(2)
   .setC(3);
  f.dump();
  return 0;
}

// vim: foldmethod=marker
