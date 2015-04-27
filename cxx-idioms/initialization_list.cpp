#include <iostream>
#include <cstdio>
struct A {
  A() { puts(__PRETTY_FUNCTION__); }
  A(int v) { puts(__PRETTY_FUNCTION__); }
};

struct B {
  A a;
  B() { a = A(18); }
};
struct C {
  A a;
  C() : a(42) {}
};

int main() {
  B b;
  C c;
  return 0;
}


// vim: foldmethod=marker
