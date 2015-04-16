#include <iostream>
int * foo(bool pred) {
  if(pred)
    return new int(1);
  else
    return nullptr;
}

int main() {
  if(auto ptr = foo(true)) {
    std::cout << *ptr << std::endl;
    delete ptr;
  }
  return 0;
}
