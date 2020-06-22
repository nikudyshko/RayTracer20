module; 

#include <cassert> 

export module vec; 

import std.core; 

template<typename T, typename U> 
concept castable = requires (T t, U u) 
{ 
	{ static_cast<T>(u) } -> T; 
	{ static_cast<U>(t) } -> U; 
}; 

template<typename T, typename U> 
concept mut_arithm = requires (T t, U u) 
{ 
	{ t + u }; 
	{ u + t }; 
	{ t - u }; 
	{ u - t }; 
	{ t * u }; 
	{ u * t }; 
	{ t / u }; 
	{ u / t }; 
}; 

template<typename T> 
concept sim_arithm = requires (T a, T b) 
{ 
	{ a++ }   -> T; 
	{ ++a }   -> T; 
	{ a-- }   -> T; 
	{ --a }   -> T; 
	{ a + b } -> T; 
	{ a - b } -> T; 
	{ a * b } -> T; 
	{ a / b } -> T; 
}; 

template<typename T> 
concept math_fun = requires (T v) 
{ 
	{ std::abs(v) }; 
	{ std::tan(v) }; 
	{ std::sqrt(v) }; 
}; 

export template<typename T = float> 
requires std::default_initializable<T> && sim_arithm<T> && math_fun<T> 
struct Vec 
{ 
	union 
	{ 
		T vec_[4]; 
		struct 
		{ 
			T x, y, z, w; 
		}; 
		struct 
		{ 
			T r, g, b, a; 
		}; 
		struct 
		{ 
			T s, t, u, v; 
		}; 
	}; 

	inline 
	Vec () : 
		x{T()}, y{T()}, z{T()}, w{T()} {} 

	inline 
	Vec (const Vec<T>& v) : 
		x{v.x}, y{v.y}, z{v.z}, w{v.w} {} 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec (U v) : 
		x{static_cast<T>(v)}, 
		y{static_cast<T>(v)}, 
		z{static_cast<T>(v)}, 
		w{static_cast<T>(v)} {} 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec (U x_, U y_, U z_, U w_ = U()) : 
		x{static_cast<T>(x_)}, 
		y{static_cast<T>(y_)}, 
		z{static_cast<T>(z_)}, 
		w{static_cast<T>(w_)} {} 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec (const Vec<U>& v) : 
		x{static_cast<T>(v.x)}, 
		y{static_cast<T>(v.y)}, 
		z{static_cast<T>(v.z)}, 
		w{static_cast<T>(v.w)} {} 

	inline 
	T& operator[] (const size_t i) 
	{ 
		assert(i < 4); 
		return vec_[i]; 
	} 

	inline 
	const T& operator[] (const size_t i) const 
	{ 
		assert(i < 4); 
		return vec_[i]; 
	} 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator= (const Vec<U>& rhs) 
	{ 
		x = static_cast<T>(rhs.x); 
		y = static_cast<T>(rhs.y); 
		z = static_cast<T>(rhs.z); 
		w = static_cast<T>(rhs.w); 
		return *this; 
	} 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator+= (const Vec<U>& rhs) 
	{ 
		x += static_cast<T>(rhs.x); 
		y += static_cast<T>(rhs.y); 
		z += static_cast<T>(rhs.z); 
		w += static_cast<T>(rhs.w); 
		return *this; 
	} 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator-= (const Vec<U>& rhs) 
	{ 
		x -= static_cast<T>(rhs.x); 
		y -= static_cast<T>(rhs.y); 
		z -= staitc_cast<T>(rhs.z); 
		w -= static_cast<T>(rhs.w); 
		return *this; 
	} 

	inline 
	T length () const { return std::sqrt(x*x + y*y + z*z + w*w); } 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec<T> normalize (U scale = static_cast<U>(1)) 
	{ 
		T s = static_cast<T>(scale)/length(); 
		return { x*s, y*s, z*s, w*s }; 
	} 
}; 

export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator* (T lhs, const Vec<U>& rhs) -> Vec<decltype(lhs*rhs.x)>
{ 
	return { lhs*rhs.x, lhs*rhs.y, 
			 lhs*rhs.z, lhs*rhs.w }; 
} 

export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator* (const Vec<T>& lhs, U rhs) -> Vec<decltype(lhs.x*rhs)> 
{ 
	return rhs*lhs; 
} 

export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator* (const Vec<T>& lhs, const Vec<U>& rhs) -> decltype(lhs.x*rhs.x) 
{ 
	return lhs.x*rhs.x + lhs.y*rhs.y + 
		   lhs.z*rhs.z + lhs.w*rhs.w; 
} 

export template<typename T, typename U> 
requires mut_arithm<T, U> && 
		 requires(T t, U u) { std::default_initializable<decltype(t*u)>; } 
inline 
auto operator^ (const Vec<T>& lhs, const Vec<U>& rhs) -> Vec<decltype(lhs.x*rhs.y)>
{ 
	return { lhs.y*rhs.z - lhs.z*rhs.y, 
			 lhs.z*rhs.x - lhs.x*rhs.z, 
			 lhs.x*rhs.y - lhs.y*rhs.x }; 
} 

export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator+ (const Vec<T>& lhs, const Vec<U>& rhs) -> Vec<decltype(lhs.x + rhs.x)> 
{ 
	return { lhs.x + rhs.x, 
			 lhs.y + rhs.y, 
			 lhs.z + rhs.z, 
			 lhs.w + rhs.w }; 
} 

export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator- (const Vec<T>& lhs, const Vec<U>& rhs) -> Vec<decltype(lhs.x - rhs.x)> 
{ 
	return { lhs.x - rhs.x, 
			 lhs.y - rhs.y, 
			 lhs.z - rhs.z, 
			 lhs.w - rhs.w }; 
} 

export template<typename T> 
requires output_op<T> 
inline 
std::ostream& operator<< (std::ostream& out, const Vec<T>& v) 
{ 
	out << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']'; 
	return out; 
} 
