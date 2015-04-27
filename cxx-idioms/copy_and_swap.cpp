#include <vector>
#include <iostream>

class Data
{
  std::vector<int> _data;
  public:
    Data(size_t n) : _data(n) {}

    Data & operator=(Data const &s)
    {
      // {{{
      Data tmp(s);
      std::swap(tmp._data, _data);
      // }}}
      return *this;
    }
    size_t size() const { return _data.size(); }
};

int main() {
  Data d(100);
  Data e(1000);
  d = e;
  std::cout << "d.size: " << d.size() << "e.size: " << e.size() << std::endl;
  return 0;
}
// vim: foldmethod=marker
