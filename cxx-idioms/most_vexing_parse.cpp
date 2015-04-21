#include <iostream>

struct my {
  friend std::ostream &operator<<(std::ostream &oss, my const &self) {
    return oss << "my";
  }
};

int main() {
  my a();
  my b;
  my c{};
  std::cout << a << std::endl
            << b << std::endl
            << c << std::endl;
  return 0;
}
