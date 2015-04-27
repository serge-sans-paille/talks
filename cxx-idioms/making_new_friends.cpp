#include <iostream>

template<typename T>
class Lonely {
   T value;
public:
   Lonely(const T& t) : value(t) {}
   friend std::ostream& operator<<(std::ostream& os, const Lonely<T>& b)
   {
      return os << __PRETTY_FUNCTION__ << std::endl << b.value;
   }
};

int main() {
  Lonely<int> l0(1);
  std::cout << l0 << std::endl;
  return 0;
}
// vim: foldmethod=marker
