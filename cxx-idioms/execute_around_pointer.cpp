#include <iostream>
#include <vector>


template<class Base, class Log>
class aspect : private Base {
  public:
  // {{{
  template <class T>
    class proxy : public Log {
      public:
        proxy (T* b) : base (b) {
          Log::before(*base);
        }
        T* operator -> () {
          return base;
        }
        ~proxy () {
          Log::after(*base);
        }
      private:
        T* base;
    };
  // }}}

  aspect(Base&& base) : Base(std::move(base)) {}

  proxy<Base> operator -> () {
    return {static_cast<Base*>(this)};
  }

  proxy<const Base> operator -> () const {
    return {static_cast<Base const*>(this)};
  }
};

struct logger {
  template<class T>
  void before(T&) { std::cout << "before" << std::endl; }
  template<class T>
  void after(T&) { std::cout << "after" << std::endl; }
};

int main()
{
  aspect<std::vector<int>, logger> vec(std::vector<int>{{1,2,3}});
  vec->push_back (10); // Note use of -> operator instead of . operator
  std::cout << const_cast<decltype(vec) const&>(vec)->front() << std::endl;
  return 0;
}
// vim: foldmethod=marker
