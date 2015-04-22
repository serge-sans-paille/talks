#include <tuple>
#include <vector>
#include <typeinfo>

template <class Filter, class... Types>
struct filter;

template <class Filter, class... Types>
struct filter<Filter, std::tuple<Types...>>
{
	typedef filter<Filter, Types...> filter_;
	using type = typename filter_::type;
	using next = typename filter_::next;
	static constexpr size_t size = filter_::size;
};

template <class Filter, bool include_T0, class T0, class... Types>
struct filter_impl;

template <class Filter, class T0, class... Types>
struct filter_impl<Filter, true, T0, Types...>
{
	using type = T0;
	using next = filter<Filter, Types...>;
	static constexpr size_t size = next::size + 1;
};

template <class Filter, class T0, class... Types>
struct filter_impl<Filter, false, T0, Types...>
{
	typedef filter<Filter, Types...> next_;
	using type = typename next_::type;
	using next = typename next_::next;
	static constexpr size_t size = next_::size; 
};

template <class Filter, class T0>
struct filter_impl<Filter, true, T0>
{
	using type = T0;
	using next = void;
	static constexpr size_t size = 1;
};

template <class Filter, class T0>
struct filter_impl<Filter, false, T0>
{
	using type = void;
	using next = void;
	static constexpr size_t size = 0;
};

template <class Filter, class T0, class... Types>
struct filter<Filter, T0, Types...>
{
	typedef filter_impl<Filter, Filter::template filter<T0>::result, T0, Types...> filtered;
	using type = typename filtered::type;
	using next = typename filtered::next;
	static constexpr size_t size = filtered::size;
};

template <size_t I, class Filter>
struct get_type
{
	using type = typename get_type<I-1, typename Filter::next>::type;
};

template <class Filter>
struct get_type<0, Filter>
{
	using type = typename Filter::type;
};

template <class Filter, class... T>
struct filter_to_tuple_impl;

template <class Filter, size_t... I>
struct filter_to_tuple_impl<Filter, std::index_sequence<I...>>
{
	using type = std::tuple<typename get_type<I, Filter>::type ...>;
};

template <class Filter>
using filter_to_tuple = typename filter_to_tuple_impl<Filter, std::make_index_sequence<Filter::size>>::type;

template <class Filter, class Tuple>
using filter_tuple = filter_to_tuple<filter<Filter, Tuple>>;

struct filter_is_integer
{
	template <class T>
	struct filter
	{
		static constexpr bool result = std::is_integral<T>::value;
	};
};

int main()
{
	using filtered_tuple = filter_tuple<filter_is_integer, std::tuple<std::string, int, std::string, short>>; 
	static_assert(std::is_same<filtered_tuple, std::tuple<int, short>>::value, "not good!");
}
