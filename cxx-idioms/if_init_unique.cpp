#include <iostream>
#include <memory>

std::unique_ptr<int> foo(bool pred) {
  if(pred)
    return std::make_unique<int>(1);
  else
    return {nullptr};
}

int main() {
  if(std::unique_ptr<int> ptr = foo(true)) {
    std::cout << *ptr << std::endl;
  }
  if(std::unique_ptr<int> ptr = foo(false)) {
    std::cout << "do not display this" << std::endl;
  }
  return 0;
}
