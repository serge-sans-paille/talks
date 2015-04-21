#include <iostream>
#include <string>
#include <tuple>

template <class T>
struct make_param
{
  make_param(T&& o): v(std::move(o)) { }
  make_param(T const& o): v(o) { }
  operator T() { return std::move(v); }
private:
  T v;
};

#define DECL_PARAM(NAME, TYPE)\
  struct NAME: make_param<TYPE> { using make_param<TYPE>::make_param; }

struct robot {

  struct param
  {
    DECL_PARAM(name, std::string);
    DECL_PARAM(age, size_t);
    DECL_PARAM(weight, size_t);
  };

  template<class... Params>
  robot(Params&& ... p)
  {
    std::initializer_list<int> v{(apply(std::forward<Params>(p)), 1)...};
    (void)v;
  }

  std::string const& name() const { return _name;}
  size_t const& age() const { return _age;}
  size_t const& weight() const { return _weight;}

private:
  void apply(param::name&& p) { _name = p; }
  void apply(param::age&& p) { _age = p; }
  void apply(param::weight&& p) { _weight = p; }

  std::string _name;
  size_t _age;
  size_t _weight;

};

std::ostream& operator<<(std::ostream& os, robot const& r) {
  return os << r.name() << ": " << r.age() << " " << r.weight() << std::endl;
}

int main() {
  robot r{robot::param::name("hello"), robot::param::age(12), robot::param::weight(4)};
  std::cout << r << std::endl;
  return 0;
}
