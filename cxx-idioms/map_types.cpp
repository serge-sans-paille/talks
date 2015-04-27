#include <tuple>
#include <vector>
#include <iostream>
#include <typeinfo>

template <class Mapper, class... Types>
struct map_impl
{
	using type = std::tuple<typename Mapper::template bind<Types>...>;
};

template <class Mapper, class... Types>
struct map_impl<Mapper, std::tuple<Types...>>
{
	using type = std::tuple<typename Mapper::template bind<Types>...>;
};

template <class Mapper, class... Types>
using map = typename map_impl<Mapper, Types...>::type;

struct map_vector
{
	template <class T>
	using bind = std::vector<T>;
};


int main()
{
	using mapped = map<map_vector, std::tuple<int, std::string>>;
	static_assert(std::is_same<mapped, std::tuple<std::vector<int>, std::vector<std::string>>>::value, "not good!");
}
// vim: foldmethod=marker
