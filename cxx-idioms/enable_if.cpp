#include <limits>
#include <iostream>
template<class T>
struct value_range :
  // {{{
  std::enable_if<std::is_integral<T>::value,
  // }}}
                                    std::integral_constant<size_t, (std::numeric_limits<T>::max() - std::numeric_limits<T>::min() + 1)>
  // {{{
                                   >::type
  // }}}
{};

int main() {
  std::cout << value_range<short>::value <<std::endl;
  std::cout << value_range<char>::value <<std::endl;
  return 0;
}

// vim: foldmethod=marker
