#pragma once

template<class T>
struct bit
{
	T field;
	constexpr bit(T field, int, int) :
		field{ field }
	{}
	constexpr bit(T index) :
		field{ T{ 1 } << index }
	{}
	constexpr bit(T begin, T end) :
		field{ ((T{ 1 } << (end - begin)) - 1) << begin }
	{}
	template<class U>
	constexpr bit<U> of(U other)
	{
		return { static_cast<U>(field & other), 0, 0 };
	}
	template<class U>
	constexpr bool is_set_in(U other)
	{
		return !!(field & other);
	}
	template<class U>
	constexpr bool is_unset_in(U other)
	{
		return !(field & other);
	}
	constexpr operator T()
	{
		return field;
	}
};

template<class T>
struct set
{
	T field;
	constexpr set(bit<T> field) :
		field{ field.field }
	{}
	template<class U>
	void of(U & other)
	{
		other |= field;
	}
};

template<class T>
struct unset
{
	T field;
	constexpr unset(bit<T> field) :
		field{ field.field }
	{}
	template<class U>
	void of(U & other)
	{
		other &= ~field;
	}
};

template<class T, class U>
struct range
{
	T begin;
	U end;
	range(T begin, U end) :
		begin{ begin },
		end{ end }
	{}

	template<class V>
	bool contains(V other)
	{
		return other >= begin && other < end;
	}
};
