#include <cstdio>
#include <vector>

template<class T>
class any {
  T member;
  public:
  any(T const& m) : member{m} { puts(__PRETTY_FUNCTION__);}
  any(T&& m) : member{std::move(m)} { puts(__PRETTY_FUNCTION__);}
  any(any const& other) : member(other.member) { puts(__PRETTY_FUNCTION__);}
};

// {{{
template<class T>
any<typename std::decay<T>::type> make_any(T&& arg) {
  return {std::forward<T>(arg)};
}
// }}}

int main() {
  auto a = make_any(std::vector<int>{{1,2,3}});
  std::vector<float> v{{1.1,2.2,3.3}};
  auto b = make_any(v);
  return 0;
};
// vim: foldmethod=marker
