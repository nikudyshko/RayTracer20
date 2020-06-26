module; 

#include <cassert> 

export module vec; 

import std.core; 

import custom_concepts; 

// Vector structure. Implements a four-component vector 
export template<typename T = float> 
requires std::default_initializable<T> && sim_arithm<T> && math_fun<T> 
struct Vec 
{ 
	// Store inner type for further checks 
	using v_type = T; 
	// Use union to make memory accessible via different ways 
	union 
	{ 
		// Access vector parameters as array 
		T vec_[4]; 
		// Access vector parameters as cartesian coordinates 
		struct 
		{ 
			T x, y, z, w; 
		}; 
		// Acces vector parameters as color components 
		struct 
		{ 
			T r, g, b, a; 
		}; 
		// Access vector parameters as texture coordinates 
		struct 
		{ 
			T s, t, u, v; 
		}; 
	}; 

	// Default constructor 
	inline 
	Vec () : 
		x{T()}, y{T()}, z{T()}, w{T()} {} 

	// Copy constructor 
	inline 
	Vec (const Vec<T>& v) : 
		x{v.x}, y{v.y}, z{v.z}, w{v.w} {} 

	// Construct vector from a single scalar value 
	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec (U v) : 
		x{static_cast<T>(v)}, 
		y{static_cast<T>(v)}, 
		z{static_cast<T>(v)}, 
		w{static_cast<T>(v)} {} 

	// Construct vector from a four scalar values 
	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec (U x_, U y_, U z_, U w_ = U()) : 
		x{static_cast<T>(x_)}, 
		y{static_cast<T>(y_)}, 
		z{static_cast<T>(z_)}, 
		w{static_cast<T>(w_)} {} 

	// Construct vector from another vector with different inner type 
	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec (const Vec<U>& v) : 
		x{static_cast<T>(v.x)}, 
		y{static_cast<T>(v.y)}, 
		z{static_cast<T>(v.z)}, 
		w{static_cast<T>(v.w)} {} 

	// Indexed access to vector components 
	inline 
	T& operator[] (const size_t i) 
	{ 
		assert(i < 4); 
		return vec_[i]; 
	} 

	// Indexed access to vector components for constant vectors 
	inline 
	const T& operator[] (const size_t i) const 
	{ 
		assert(i < 4); 
		return vec_[i]; 
	} 

	// Assignment operator 
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

	// Sum-assignment operator 
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

	// Substract-assignment operator 
	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator-= (const Vec<U>& rhs) 
	{ 
		x -= static_cast<T>(rhs.x); 
		y -= static_cast<T>(rhs.y); 
		z -= static_cast<T>(rhs.z); 
		w -= static_cast<T>(rhs.w); 
		return *this; 
	} 

	// Calculate the lentgh of vector 
	inline 
	T length () const { return std::sqrt(x*x + y*y + z*z + w*w); } 

	// Normalization of vector 
	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec<T> normalize (U scale = static_cast<U>(1)) 
	{ 
		T s = static_cast<T>(scale)/length(); 
		return { x*s, y*s, z*s, w*s }; 
	} 
}; 

// Scalar-vector multiplication 
export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator* (T lhs, const Vec<U>& rhs) -> Vec<decltype(lhs*rhs.x)>
{ 
	return { lhs*rhs.x, lhs*rhs.y, 
			 lhs*rhs.z, lhs*rhs.w }; 
} 

// Vector-scalar multiplication 
export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator* (const Vec<T>& lhs, U rhs) -> Vec<decltype(lhs.x*rhs)> 
{ 
	return rhs*lhs; 
} 

// Vector-Vector dot-product 
export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator* (const Vec<T>& lhs, const Vec<U>& rhs) -> decltype(lhs.x*rhs.x) 
{ 
	return lhs.x*rhs.x + lhs.y*rhs.y + 
		   lhs.z*rhs.z + lhs.w*rhs.w; 
} 

// Vector-Vector cross-product 
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

// Vector-Vector addition 
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

// Vector-Vector substraction 
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

// Vector output operator 
export template<typename T> 
requires output_op<T> 
inline 
std::ostream& operator<< (std::ostream& out, const Vec<T>& v) 
{ 
	out << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']'; 
	return out; 
} 
