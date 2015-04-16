#include <iostream>
#include <string>
#include <tuple>

struct robot {
  struct param {
    using name = std::string;
    using age = size_t;
  };

  template<class... Params>
  robot(Params const&... p) : robot(std::tuple<Params...>(p...)) {
  }

  param::name const& name() const { return _name;}
  param::age const& age() const { return _age;}

  protected:
  template<class... Params>
  robot(std::tuple<Params...> const& params) :
    _name(std::get<param::name>(params)),
    _age(std::get<param::age>(params))
  {
  }
  param::name _name;
  param::age _age;

};

std::ostream& operator<<(std::ostream& os, robot const& r) {
  return os << r.name() << ": " << r.age() << std::endl;
}


int main() {
  robot r{robot::param::name("hello"), robot::param::age(12)};
  std::cout << r << std::endl;
  return 0;
}
