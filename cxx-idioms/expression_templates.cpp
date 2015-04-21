#include <iostream>
#include <cstdio>
#include <string>
#include <utility>

namespace exp {
class string {

  std::string _str;

public:
  string(std::string const &s) : _str(s) { puts(__PRETTY_FUNCTION__); }
  string(std::string &&s) : _str(std::move(s)) { puts(__PRETTY_FUNCTION__); }
  std::string const &str() const { return _str; }
  void append(std::string &buffer) const { buffer += _str; }
  size_t size() const { return _str.size(); }
};

template <class Lhs, class Rhs>
struct string_chain : std::pair<Lhs, Rhs> {
  string_chain(Lhs lhs, Rhs rhs) : std::pair<Lhs, Rhs>{lhs, rhs} {
    puts(__PRETTY_FUNCTION__);
  }

  std::string str() const {
    std::string buffer;
    buffer.reserve(size());
    append(buffer);
    return buffer;
  }
  void append(std::string &buffer) const {
    std::get<0>(*this).append(buffer);
    buffer += ' ';
    std::get<1>(*this).append(buffer);
  }
  size_t size() const { return std::get<0>(*this).size() + 1 + std::get<1>(*this).size(); }
};

template<class Lhs, class Rhs>
string_chain<Lhs, Rhs> operator^(Lhs&& lhs, Rhs&& rhs) {
  return {std::forward<Lhs>(lhs), std::forward<Rhs>(rhs)};
}
}

int main(int argc, char* argv[]) {
  std::cout << (exp::string("voici") ^ exp::string("venir") ^ exp::string("les") ^  exp::string("barbapa")).str() << std::endl;
  return 0;
}
