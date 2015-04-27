#include <iostream>

namespace exp {

class UniquePtr {
  int * _ptr;
  public:
  UniquePtr(int* ptr) : _ptr(ptr) {};
  UniquePtr(UniquePtr const&) = delete;
  UniquePtr& operator=(UniquePtr const&) = delete;
  ~UniquePtr() { delete _ptr; }

  int& operator*() { return *_ptr;}
  int operator*() const { return *_ptr;}

  void swap(UniquePtr& other) {
    std::swap(_ptr, other._ptr);
  }
};

}

namespace std {
template<>
void swap<exp::UniquePtr>(exp::UniquePtr& self, exp::UniquePtr& other) {
  self.swap(other);
}
}


int main() {
  exp::UniquePtr a { new int(0xA) };
  exp::UniquePtr b { new int(0xB) };
  std::cout << std::hex << "a: 0x" << *a << " b: 0x" <<* b << std::endl;
  std::swap(a,b);
  std::cout << std::hex << "a: 0x" << *a << " b: 0x" <<* b << std::endl;
  using namespace std;
  swap(a,b);
  std::cout << std::hex << "a: 0x" << *a << " b: 0x" <<* b << std::endl;
  return 0;
}



// vim: foldmethod=marker
