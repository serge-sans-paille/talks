#include <cstdio>

struct Foo {
  const int value;

  Foo(int v) : value(v) {}
  Foo(Foo&& foo)
  : value(foo.value)
  {
    puts(__PRETTY_FUNCTION__);
  }
  Foo(Foo const& foo)
  : value(foo.value)
  {
    puts(__PRETTY_FUNCTION__);
  }
};

Foo test() {
  Foo f(0);
  return f;
}

int main() {
  Foo g(test());
  return g.value;
}

// vim: foldmethod=marker
