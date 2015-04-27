#include <iostream>
#include <tuple>
#include <vector>

template <class ObjData>
struct ObjAccessVertical: public ObjData
{
	using ObjData::ObjData;

	template <size_t I>
	auto& get_data()
	{
		return std::get<I>(static_cast<ObjData&>(*this));
	}

	template <size_t I>
	auto const& get_data() const
	{
		return std::get<I>(static_cast<ObjData const&>(*this));
	}
};

template <template <class Storage> class Obj, class... Args>
struct ObjStorageVertical: public std::vector<Obj<ObjAccessVertical<std::tuple<Args...>>>>
{ };

template <class Storage>
struct ObjAccessHorizontal
{
	ObjAccessHorizontal(Storage* s, size_t i):
		storage(s),
		idx(i)
	{ }

	ObjAccessHorizontal(ObjAccessHorizontal const& o):
		ObjAccessHorizontal(o.storage, o.idx)
	{ }

	template <size_t I>
	auto& get_data()
	{
		return std::get<I>(*storage)[idx];
	}

	template <size_t I>
	auto const& get_data() const
	{
		return std::get<I>(*storage)[idx];
	}

	Storage* storage;
	size_t idx;
};

template <template <class Storage> class Obj, class... Args>
struct ObjStorageHorizontal
{
	typedef std::tuple<std::vector<Args>...> storage_type;
	typedef Obj<ObjAccessHorizontal<storage_type>> obj_type;

	static std::make_index_sequence<sizeof...(Args)> args_ns;

	struct iterator
	{
		iterator() = default;

		obj_type& operator*() { return _o; }
		obj_type& operator->() { return _o; }


		bool operator==(iterator const& v) const
		{
			return _o.idx == v._o.idx && _o.storage == v._o.storage;
		}

		bool operator!=(iterator const& v) const
		{
			return !(*this == v);
		}

		iterator& operator++(int)
		{
			iterator ret(*this);
			++_o.idx;
			return ret;
		}

		iterator& operator++()
		{
			++_o.idx;
			return *this;
		}

		iterator operator+(size_t const n) const
		{
			return iterator{obj_type{_o.storage, _o.idx+n}};
		}

		obj_type _o;
	};

	void emplace_back(Args&& ... args)
	{
		emplace_back(std::forward<Args>(args)..., args_ns);
	}

	template <size_t... I>
	void emplace_back(Args&& ... args, std::index_sequence<I...>)
	{
		std::initializer_list<int> li = {(std::get<I>(_storage).emplace_back(std::forward<Args>(args)), 1)...};
		(void)li;
	}

	void reserve(size_t n)
	{
		reserve(n, args_ns);
	}

	template <size_t... I>
	void reserve(size_t n, std::index_sequence<I...>)
	{
		std::initializer_list<int> li = {(std::get<I>(_storage).reserve(n), 1)...};
		(void)li;
	}

	void resize(size_t n)
	{
		resize(n, args_ns);
	}

	template <size_t... I>
	void resize(size_t n, std::index_sequence<I...>)
	{
		std::initializer_list<int> li = {(std::get<I>(_storage).resize(n), 1)...};
		(void)li;
	}

	iterator begin()
	{
		return iterator{obj_type{&_storage, 0}};
	}

	iterator end()
	{
		return iterator{obj_type{&_storage, std::get<0>(_storage).size()}};
	}

	obj_type front() { return *begin(); }

	obj_type operator[](size_t idx) { return *(begin()+idx); }

private:
	storage_type _storage;
};

template <size_t I, class O>
auto& get_data(O& o)
{
	return o.template get_data<I>();
}

template <size_t I, class O>
auto const& get_data(O const& o)
{
	return o.template get_data<I>();
}

// Public interface
//

template <class Accessor>
struct Obj: public Accessor
{
	using Accessor::Accessor;

	void dump()
	{
		std::cout << name() << " is " << age() << std::endl;
	}

	int& age() { return get_data<0>(*this); }
	std::string& name() { return get_data<1>(*this); }

	int age() const { return get_data<0>(*this); }
	std::string name() const { return get_data<1>(*this); }
};


template <class T>
void test(T& s)
{
	s.reserve(4);
	s.emplace_back(1, "blabla");
	s.emplace_back(2, "blabla");
	s.emplace_back(3, "blabla");
	s.emplace_back(4, "blabla");
	s.front().dump();
	s.front().age() = 10;
	for (auto& o: s) {
		o.dump();
		o.name() = "test";
	}
	for (auto& o: s) {
		o.dump();
	}
}

template <class T>
 __attribute__((noinline)) void test_vec(size_t n)
{
	T h0,h1,hr;
	h0.resize(n);
	h1.resize(n);
	hr.resize(n);

	for (size_t i = 0; i < n; i++) {
		hr[i].age() = h0[i].age() + h1[i].age();
	}
}

int main(int argc, char** argv)
{
	ObjStorageVertical<Obj, int, std::string> V;
	test(V);

	std::cout << "--" << std::endl;

	ObjStorageHorizontal<Obj, int, std::string> H;
	test(H);

	const size_t n = atoll(argv[1]);
	test_vec<decltype(V)>(n);
	test_vec<decltype(H)>(n);
	return 0;
}
// vim: foldmethod=marker
