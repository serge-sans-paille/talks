#include <iostream>

long dump(int i) { return i ; }

template <class T>
typename T::type dump(T t)
{
  return t;
}
int main()
{
  std::cout << dump(1.5) << std::endl;
  return 0;
}
// vim: foldmethod=marker
