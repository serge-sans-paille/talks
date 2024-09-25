#include <iostream>
#include <vector>

// {{{
namespace impl {
template<class Iter>
class range {
  Iter _begin, _end;
  public:
  range(Iter b, Iter e) : _begin(b), _end(e) {}
  Iter begin() { return _begin; }
  Iter end() { return _end; }
};
}

template<class Iter>
impl::range<Iter> range(Iter b, Iter e) { return {b, e};}
// }}}

int main() {
  std::vector<int> V{{1,2,3,4,5}};
  for(auto value :
      // {{{
      range(V.begin(), V.end() - 2)
      // }}}
      )
    std::cout << value << std::endl;
  return 0;
}
// vim: foldmethod=marker
