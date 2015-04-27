#include <cassert>
#include <iostream>

template<class T>
class OptionType {
  char  _storage[sizeof(T)];
  T* _value;

  public:
  OptionType() : _value(nullptr) {}
  OptionType(OptionType const&) = delete;
  OptionType(OptionType && other)
  {
    if(other._value)
      _value = new (_storage) T(std::move(*other._value));
    else
      _value = nullptr;
  }
  template<class... Args>
  OptionType(Args&& ... args)
    : _value{new (_storage) T(std::forward<Args>(args)...)}
  {
  }

  operator bool() const { return _value; }

  T& get() {
    assert(_value && "not in error state");
    return *_value;
  }
  T const& get() const {
    assert(_value && "not in error state");
    return *_value;
  }
};

OptionType<int> maybe(int value) {
  if(value != 0)
    return 5 / value;
  else
    return {};
};

int main() {
  if(auto v = maybe(0)) {
    std::cout << v.get() << std::endl;
  }
  else {
    std::cerr << "error handling code" << std::endl;
  }
  if(auto v = maybe(1)) {
    std::cout << v.get() << std::endl;
  }
  else {
    std::cerr << "error handling code" << std::endl;
  }
  return 0;
}



// vim: foldmethod=marker
