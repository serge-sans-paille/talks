#include <iostream>
#include <vector>


template<class Base, class Log>
class aspect : private Base {
  public:
    class proxy : public Log {
      public:
        proxy (Base* b) : base (b) {
          Log::before(*base);
        }
        Base * operator -> () {
          return base;
        }
        ~proxy () {
          Log::after(*base);
        }
      private:
        Base * base;
    };

    aspect(Base&& base) : Base(std::move(base)) {}

    proxy operator -> () {
       return {static_cast<Base*>(this)};
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
  return 0;
}
