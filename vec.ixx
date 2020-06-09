module; 

#include <cassert> 

import std.core; 

export module vec; 

template<typename T, typename U> 
concept castable = requires(T t, U u) 
{ 
	{ static_cast<T>(u) } -> T; 
	{ static_cast<U>(t) } -> U; 
}; 

template<typename T, typename U> 
concept mut_arithm = requires(T t, U u) 
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
concept sim_arithm = requires(T a, T b) 
{ 
	{ a + b } -> T; 
	{ a - b } -> T;  
	{ a * b } -> T;  
	{ a / b } -> T;  
}; 

template<typename T> 
concept rootable = requires(T v) 
{ { std::sqrt(v) }; }; 

export template<typename T = float> 
requires std::default_initializable<T> && sim_arithm<T> && rootable<T> 
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
	}; 

	inline Vec() : 
		x{T()}, y{T()}, z{T()}, w{T()} { /*std::cout << "Default" << x << y << z << w << '\n';*/ } 

	inline Vec(std::initializer_list<T> l) 
	{ 
		assert((l.size() == 1) || (l.size() >= 3)); 
		auto it = l.begin(); 
		if (l.size() == 1) 
		{ 
			x = y = z = w = *it;  
		} 
		else 
			if (l.size() == 3) 
			{ 
				x = *it; 
				y = *(it + 1); 
				z = *(it + 2); 
				w = T(); 
			} 
			else 
				if (l.size() == 4) 
				{ 
					x = *it; 
					y = *(it + 1); 
					z = *(it + 2); 
					w = *(it + 3); 
				} 
//		std::cout << "Sim Init List" << x << y << z << w << '\n'; 
	}  

	inline Vec(const Vec<T>& v) : 
		x{v.x}, y{v.y}, z{v.z}, w{v.w} { /*std::cout << "From Sim Vec" << x << y << z << w << '\n';*/ } 

	template<typename U = T> 
	requires castable<T, U> 
	inline Vec(U v) : 
		x{static_cast<T>(v)}, 
		y{static_cast<T>(v)}, 
		z{static_cast<T>(v)}, 
		w{static_cast<T>(v)} { /*std::cout << "Single" << x << y << z << w << '\n';*/ } 

	template<typename U = T> 
	requires castable<T, U> 
	inline Vec(U x_, U y_, U z_, U w_ = U()) : 
		x{static_cast<T>(x_)}, 
		y{static_cast<T>(y_)}, 
		z{static_cast<T>(z_)}, 
		w{static_cast<T>(w_)} { /*std::cout << "Multiple" << x << y << z << w << '\n';*/ } 

	template<typename U = T> 
	requires castable<T, U> 
	inline Vec(std::initializer_list<U> l) 
	{ 
		assert((l.size() == 1) || (l.size() >= 3)); 
		auto it = l.begin(); 
		if (l.size() == 1) 
		{ 
			x = y = z = w = *it; 
		} 
		else 
			if (l.size() == 3) 
			{ 
				x = *it; 
				y = *(it + 1); 
				z = *(it + 2); 
				w = T(); 
			} 
			else 
				if (l.size() == 4) 
				{ 
					x = *it; 
					y = *(it + 1); 
					z = *(it + 2); 
					w = *(it + 3); 
				} 
//		std::cout << "Mut Init List" << x << y << z << w << '\n'; 
	} 

	template<typename U = T> 
	requires castable<T, U> 
	inline Vec(const Vec<U>& v) : 
		x{static_cast<T>(v.x)}, 
		y{static_cast<T>(v.y)}, 
		z{static_cast<T>(v.z)}, 
		w{static_cast<T>(v.w)} { /*std::cout << "From Mut Vec" << x << y << z << w << '\n';*/ } 

	inline T& operator[](const size_t i) 
	{ 
		assert(i < 4); 
		return vec_[i]; 
	} 

	inline const T& operator[](const size_t i) const 
	{ 
		assert(i < 4); 
		return vec_[i]; 
	} 

	inline T length() const { return std::sqrt(x*x + y*y + z*z + w*w); } 

	template<typename U = T> 
	requires castable<T, U> 
	inline Vec<T> normalize(U scale = static_cast<U>(1)) 
	{ 
		T s = static_cast<T>(scale)/length(); 
		return {x*s, y*s, z*s, w*s}; 
	} 

	template<typename U = T> 
	requires castable<T, U> 
	inline Vec<T>& operator=(const Vec<U>& rhs) 
	{ 
		x = static_cast<T>(rhs.x); 
		y = static_cast<T>(rhs.y); 
		z = static_cast<T>(rhs.z); 
		w = static_cast<T>(rhs.w); 
		return *this; 
	} 

	template<typename U = T> 
	requires castable<T, U> 
	inline Vec<T>& operator+=(const Vec<U>& rhs) 
	{ 
		x += static_cast<T>(rhs.x); 
		y += static_cast<T>(rhs.y); 
		z += static_cast<T>(rhs.z); 
		w += static_cast<T>(rhs.w); 
		return *this; 
	} 

	template<typename U = T> 
	requires castable<T, U> 
	inline Vec<T>& operator-=(const Vec<U>& rhs) 
	{ 
		x -= static_cast<T>(rhs.x); 
		y -= static_cast<T>(rhs.y); 
		z -= static_cast<T>(rhs.z); 
		w -= static_cast<T>(rhs.w); 
		return *this; 
	} 
}; 

export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline auto operator*(const Vec<T>& lhs, const Vec<U>& rhs) -> decltype(lhs.x*rhs.x) 
{ 
	return lhs.x*rhs.x + lhs.y*rhs.y + 
		   lhs.z*rhs.z + lhs.w*rhs.w; 
} 

export template<typename T, typename U> 
requires mut_arithm<T, U> && 
		 requires(T t, U u) { std::default_initializable<decltype(t*u)>; } 
inline auto operator^(const Vec<T>& lhs, const Vec<U>& rhs) -> Vec<decltype(lhs.x*rhs.y)> 
{ 
	using V = decltype(lhs.x*rhs.y); 
	V x = lhs.y*rhs.z - lhs.z*rhs.y; 
	V y = lhs.z*rhs.x - lhs.x*rhs.z; 
	V z = lhs.x*rhs.y - lhs.y*rhs.x; 
	V w = V(); 
	return Vec<V>(x, y, z, w); 
} 

export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline auto operator+(const Vec<T>& lhs, const Vec<U>& rhs) -> Vec<decltype(lhs.x + rhs.x)> 
{ 
	using V = decltype(lhs.x + rhs.x); 
	return Vec<V>(lhs.x + rhs.x, lhs.y + rhs.y, 
				  lhs.z + rhs.z, lhs.w + rhs.w); 
} 

export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline auto operator-(const Vec<T>& lhs, const Vec<U>& rhs) -> Vec<decltype(lhs.x - rhs.x)> 
{ 
	using V = decltype(lhs.x - rhs.x); 
	return Vec<V>(lhs.x - rhs.x, lhs.y - rhs.y, 
				  lhs.z - rhs.z, lhs.w - rhs.w); 
} 

export template<typename T> 
inline std::ostream& operator<<(std::ostream& out, const Vec<T>& v) 
{ 
	out << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']'; 
	return out; 
}  
