#include <cstdio>
// {{{
template <class T>
struct Base
{
  void implementation()
  {
    puts(__PRETTY_FUNCTION__);
    static_cast<T*>(this)->implementation();
  }
};
// }}}

struct Derived :
  // {{{
  Base<Derived>
  // }}}
{
  void implementation()
  {
    puts(__PRETTY_FUNCTION__);
  };
};

int main() {

  Derived d;
  d.implementation();
  puts("===");
  // {{{
  Base<Derived>
  // }}}
    & b = d;
  b.implementation();
  return 0;
}
// vim: foldmethod=marker
