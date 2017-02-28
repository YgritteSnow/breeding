#pragma once

template<class... Ts> struct MEM_MAX;
template<> struct MEM_MAX<> { static constexpr const int SIZE = 0; };
template <typename T1, typename... Tn> struct MEM_MAX<T1, Tn...> { static constexpr const int SIZE = MEM_MAX<Tn...>::SIZE > sizeof(T1) ? MEM_MAX<Tn...>::SIZE : sizeof(T1); };

class variant
{
public:
	variant();
	explicit variant(int v);
	explicit variant(float v);
	explicit variant(bool v);
	variant(const variant& other);

	explicit operator int() const;
	explicit operator float() const;
	explicit operator bool() const;

	variant& operator=(int v);
	variant& operator=(float v);
	variant& operator=(bool v);
	variant& operator=(const variant& rhs);
	variant& operator+=(const variant& rhs);
	variant& operator-=(const variant& rhs);

	bool operator>=(const variant& rhs) const;
	bool operator<=(const variant& rhs) const;
	bool operator>(const variant& rhs) const;
	bool operator<(const variant& rhs) const;
	bool operator==(const variant& rhs) const;
	bool operator!=(const variant& rhs) const;
	bool operator!() const;
	
	enum VAR_TYPE { VT_INT, VT_FLOAT, VT_BOOL, VT_NONE };
	variant::VAR_TYPE get_type() const;

	template <typename T>
	inline T as() const { return (T)(*this);  }

private:
	typedef unsigned char byte;
	typedef MEM_MAX<int, float, bool> MMAX;

	variant::VAR_TYPE m_value_type;
	variant::byte m_value_buffer[MMAX::SIZE];
};

