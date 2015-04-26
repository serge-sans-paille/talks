#include <iostream>
#include <tuple>

auto list = [](auto ...xs) {
    return [=](auto access) { return access(xs...); };
};

auto length = [](auto xs) {
    return xs([](auto ...z) { return sizeof...(z); });
};

template<size_t I, class T>
auto get(T&& xs) {
    return xs([](auto ...z) { return std::get<I>(std::make_tuple(z...)); });
}

int main()
{
  auto tuple_int = list(int(1), short(2), char(3));
  std::cout << sizeof(tuple_int) << " / " << sizeof(std::tuple<int, short, char>) << std::endl;

  auto tuple = list(1, '2', "3");
  std::cout << length(tuple) << std::endl;
  std::cout << get<1>(tuple) << std::endl;
  return 0;
}
