#include <iostream>
#include <stdexcept>
struct Eldrazi {
  Eldrazi() { throw std::runtime_error("not of this world");}
};

struct Deck {
  Eldrazi _eldrazi;

  Deck() try : _eldrazi() {} catch(...) { std::cerr << "surprise" << std::endl; }
};

void foo()
  try {
    Eldrazi{};
  }
  catch(...) {
    std::cerr << "unusual" << std::endl;
  }

int main() {
  foo();
  Deck d;
  return 0;
}
