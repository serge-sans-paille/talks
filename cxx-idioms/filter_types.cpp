#include <tuple>

namespace details {

template<class Pred, class...> struct filter;
template<class Pred, class Head, class... Tail>
struct filter<Pred, Head, Tail...> {
  using tail_type = typename filter<Pred, Tail...>::type;
  using type = typename std::conditional<Pred::template apply<Head>::value,
                                         decltype(std::tuple_cat(std::tuple<Head>(), tail_type())),
                                         tail_type
                                        >::type;
};
template<class Pred>
struct filter<Pred> {
  using type = std::tuple<>;
};

}

struct is_integral {
  template<class T>
  using apply = std::is_integral<T>;
};

template<class Pred, class Tuple> struct filter_tuple;
template<class Pred, class... Args>
struct filter_tuple<Pred, std::tuple<Args...>>
  : details::filter<Pred, Args...>
{
};


int main()
{
  using filtered_tuple = filter_tuple<is_integral, std::tuple<std::string, int, std::string, short>>::type;
  static_assert(std::is_same<filtered_tuple, std::tuple<int, short>>::value, "not good!");
}
