#include <iostream>
#include <iterator>
#include <algorithm>

int main() {
  std::fill_n(std::ostream_iterator<char>(std::cout), 10, '-');
  std::cout << std::endl;
  return 0;
}
