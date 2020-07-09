module; 

#include <cassert> 

export module vec; 

import std.core; 

import custom_concepts; 

// Vector structure. Implements a four-component vector 
export template<typename T> 
requires std::default_initializable<T> && sim_arithm<T> && math_fun<T> 
struct Vec 
{ 
	// Store inner type for further checks 
	using v_type = T; 
	// Use union to make memory accessible via different ways 
	union 
	{ 
		// Access vector parameters as array 
		T vec_[4]{}; 
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

	// Move constructor 
	inline 
	Vec (Vec<T>&& v) : 
		x{v.x}, y{v.y}, z{v.z}, w{v.w} {} 

	// Copy assignment operator 
	inline 
	Vec<T>& operator= (const Vec<T>& rhs) 
	{ 
		x = rhs.x; 
		y = rhs.y; 
		z = rhs.z; 
		w = rhs.w; 
		return *this; 
	} 

	// Move assignment operator 
	inline 
	Vec<T>& operator= (const Vec<T>&& rhs) 
	{ 
		x = rhs.x; 
		y = rhs.y; 
		z = rhs.z; 
		w = rhs.w; 
		return *this; 
	}

	// Construct vector from a single scalar value 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec (U v) : 
		x{T(v)}, y{T(v)}, z{T(v)}, w{T(v)} {} 

	// Construct vector from a four scalar values 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec (U x_, U y_, U z_, U w_ = U()) : 
		x{T(x_)}, y{T(y_)}, z{T(z_)}, w{T(w_)} {} 

	// Construct vector from another vector with different inner type 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec (const Vec<U>& v) : 
		x{T(v.x)}, y{T(v.y)}, z{T(v.z)}, w{T(v.w)} {} 

	// Construct vector from another vector rvalue with different inner type 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec (Vec<U>&& v) : 
		x{T(v.x)}, y{T(v.y)}, z{T(v.z)}, w{T(v.w)} {} 

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

	// Scalar assignment operator 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator= (U rhs) 
	{ 
		x = y = z = w = T(rhs); 
		return *this; 
	}

	// Assignment operator 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator= (const Vec<U>& rhs) 
	{ 
		x = T(rhs.x); 
		y = T(rhs.y); 
		z = T(rhs.z); 
		w = T(rhs.w); 
		return *this; 
	} 

	// Move-assignment operator with rvalue 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator= (Vec<U>&& rhs) 
	{ 
		x = T(rhs.x); 
		y = T(rhs.y); 
		z = T(rhs.z); 
		w = T(rhs.w); 
		return *this; 
	} 

	// Scalar sum-assignment operator 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator+= (U rhs) 
	{ 
		x += T(rhs); 
		y += T(rhs); 
		z += T(rhs); 
		w += T(rhs); 
		return *this; 
	} 

	// Sum-assignment operator 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator+= (const Vec<U>& rhs) 
	{ 
		x += T(rhs.x); 
		y += T(rhs.y); 
		z += T(rhs.z); 
		w += T(rhs.w); 
		return *this; 
	} 

	// SUm-assignment operator with rvalue 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator+= (const Vec<U>&& rhs) 
	{ 
		x += T(rhs.x); 
		y += T(rhs.y); 
		z += T(rhs.z); 
		w += T(rhs.w);  
		return *this; 
	} 

	// Scalar substract-assignment operator 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator-= (U rhs) 
	{ 
		x -= T(rhs); 
		y -= T(rhs); 
		z -= T(rhs); 
		w -= T(rhs); 
		return *this; 
	}

	// Substract-assignment operator 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator-= (const Vec<U>& rhs) 
	{ 
		x -= T(rhs.x); 
		y -= T(rhs.y); 
		z -= T(rhs.z); 
		w -= T(rhs.w); 
		return *this; 
	} 

	// Substract-assignment operator with rvalue 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator-= (const Vec<U>&& rhs) 
	{ 
		x -= T(rhs.x); 
		y -= T(rhs.y); 
		z -= T(rhs.z); 
		w -= T(rhs.w); 
		return *this; 
	} 

	// Scalar multiply-assignment operator 
	template<typename U> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator*= (U rhs) 
	{ 
		x *= T(rhs); 
		y *= T(rhs); 
		z *= T(rhs); 
		w *= T(rhs); 
		return *this; 
	} 

	// Calculate the lentgh of vector 
	inline 
	T length () const { return std::sqrt(x*x + y*y + z*z + w*w); } 

	// Normalization of vector 
	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec<T> normalize (U scale = U(1)) 
	{ 
		T s = T(scale)/length(); 
		return { x*s, y*s, z*s, w*s }; 
	} 
}; 

// Negation operator 
export template<typename T> 
requires sim_arithm<T> 
inline 
Vec<T> operator- (const Vec<T>& v) 
{ 
	return { -v.x, -v.y, -v.z, -v.w }; 
} 

// Negation operator with rvalue 
export template<typename T> 
requires sim_arithm<T> 
inline 
Vec<T>& operator- (Vec<T>&& v) 
{ 
	v.x = -v.x; 
	v.y = -v.y; 
	v.z = -v.z; 
	v.w = -v.w; 
	return v; 
} 

// Scalar-Vector multiplication 
export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator* (T lhs, const Vec<U>& rhs) -> Vec<decltype(lhs*rhs.x)>
{ 
	return { lhs*rhs.x, lhs*rhs.y, 
			 lhs*rhs.z, lhs*rhs.w }; 
} 

// Scalar-Vector multiplication with rvalue 
export template<typename T, typename U> 
requires castable<T, U> && sim_arithm<U> 
inline 
auto operator* (T lhs, Vec<U>&& rhs) -> Vec<U>& 
{ 
	rhs.x *= U(lhs); 
	rhs.y *= U(lhs); 
	rhs.z *= U(lhs); 
	rhs.w *= U(lhs); 
	return rhs; 
} 

// Vector-Scalar multiplication 
export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator* (const Vec<T>& lhs, U rhs) -> Vec<decltype(lhs.x*rhs)> 
{ 
	return { lhs.x*rhs, lhs.y*rhs, 
			 lhs.z*rhs, lhs.w*rhs }; 
} 

// Vector-Scalar multiplication with rvalue 
export template<typename T, typename U> 
requires castable<T, U> && sim_arithm<T> 
inline 
auto operator* (Vec<T>&& lhs, U rhs) -> Vec<T>& 
{ 
	lhs.x *= T(rhs); 
	lhs.y *= T(rhs); 
	lhs.z *= T(rhs); 
	lhs.w *= T(rhs); 
	return lhs; 
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

// Vector-Vector dot-product with rvalue 
export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator* (const Vec<T>& lhs, const Vec<U>&& rhs) -> decltype(lhs.x*rhs.x) 
{ 
	return lhs.x*rhs.x + lhs.y*rhs.y + 
		   lhs.z*rhs.z + lhs.w*rhs.w; 
} 

// Vector-Vector dot-product with rvalue 
export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator* (const Vec<T>&& lhs, const Vec<U>& rhs) -> decltype(lhs.x*rhs.x) 
{ 
	return lhs.x*rhs.x + lhs.y*rhs.y + 
		   lhs.z*rhs.z + lhs.w*rhs.w; 
} 

// Vector-Vector dot-product with rvalues 
export template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator* (const Vec<T>&& lhs, const Vec<U>&& rhs) -> decltype(lhs.x*rhs.x) 
{ 
	return lhs.x*rhs.x + lhs.y*rhs.y + 
		   lhs.z*rhs.z + lhs.w*rhs.w; 
}

// Vector-Vector cross-product 
export template<typename T, typename U> 
requires mut_arithm<T, U> && 
		 requires (T t, U u) { std::default_initializable<decltype(t*u)>; } 
inline 
auto operator^ (const Vec<T>& lhs, const Vec<U>& rhs) -> Vec<decltype(lhs.x*rhs.y)>
{ 
	return { lhs.y*rhs.z - lhs.z*rhs.y, 
			 lhs.z*rhs.x - lhs.x*rhs.z, 
			 lhs.x*rhs.y - lhs.y*rhs.x }; 
} 

// Vector-Vector cross-product with rvalue 
export template<typename T, typename U> 
requires castable<T, U> && sim_arithm<U> 
inline 
auto operator^ (const Vec<T>& lhs, Vec<U>&& rhs) -> Vec<U>& 
{ 
	rhs = { U(lhs.y)*rhs.z - U(lhs.z)*rhs.y, 
			U(lhs.z)*rhs.x - U(lhs.x)*rhs.z, 
			U(lhs.x)*rhs.y - U(lhs.y)*rhs.x, 
			U()	}; 
	return rhs; 
} 

// Vector-Vector cross-product with rvalue 
export template<typename T, typename U> 
requires castable<T, U> && sim_arithm<T> 
inline 
auto operator^ (Vec<T>&& lhs, const Vec<U>& rhs) -> Vec<T>& 
{ 
	lhs = { lhs.y*T(rhs.z) - lhs.z*T(rhs.y), 
			lhs.z*T(rhs.x) - lhs.x*T(rhs.z), 
			lhs.x*T(rhs.y) - lhs.y*T(rhs.x), 
			T() }; 
	return lhs; 
} 

// Vector-Vector cross-product with rvalues 
export template<typename T, typename U> 
requires castable<T, U> && sim_arithm<T> && sim_arithm<U> 
inline 
auto operator^ (Vec<T>&& lhs, Vec<U>&& rhs) -> Vec<decltype(lhs.x*rhs.y)> 
{ 
	using W = decltype(lhs.x*rhs.y); 
	if constexpr(std::is_same<W, T>) 
	{ 
		lhs = { lhs.y*T(rhs.z) - lhs.z*T(rhs.y), 
				lhs.z*T(rhs.x) - lhs.x*T(rhs.z), 
				lhs.x*T(rhs.y) - lhs.y*T(rhs.x), 
				T() }; 
		return lhs; 
	} 
	else 
	{ 
		rhs = { U(lhs.y)*rhs.z - U(lhs.z)*rhs.y, 
				U(lhs.z)*rhs.z - U(lhs.x)*rhs.z, 
				U(lhs.x)*rhs.y - U(lhs.y)*rhs.x, 
				U() }; 
		return rhs; 
	} 
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

// Vector-Vector addition with rvalue 
export template<typename T, typename U> 
requires castable<T, U> && sim_arithm<U> 
inline 
auto operator+ (const Vec<T>& lhs, Vec<U>&& rhs) -> Vec<U>& 
{ 
	rhs.x += U(lhs.x); 
	rhs.y += U(lhs.y); 
	rhs.z += U(lhs.z); 
	rhs.w += U(lhs.w); 
	return rhs; 
} 

// Vector-Vector addition with rvalue 
export template<typename T, typename U> 
requires castable<T, U> && sim_arithm<U> 
inline 
auto operator+ (Vec<T>&& lhs, const Vec<U>& rhs) -> Vec<T>& 
{ 
	lhs.x += T(rhs.x); 
	lhs.y += T(rhs.y); 
	lhs.z += T(rhs.z); 
	lhs.w += T(rhs.w); 
	return lhs; 
} 

// Vector-Vector addition with rvalues 
export template<typename T, typename U> 
requires castable<T, U> && sim_arithm<T> && sim_arithm<U> 
inline 
auto operator+ (Vec<T>&& lhs, Vec<U>&& rhs) -> Vec<decltype(lhs.x + rhs.x)>& 
{ 
	using W = decltype(lhs.x + rhs.x); 
	if constexpr(std::is_same<T, W>::value) 
	{ 
		lhs.x += T(rhs.x); 
		lhs.y += T(rhs.y); 
		lhs.z += T(rhs.z); 
		lhs.w += T(rhs.w); 
		return lhs; 
	} 
	else 
	{ 
		rhs.x += U(lhs.x); 
		rhs.y += U(lhs.y); 
		rhs.z += U(lhs.z); 
		rhs.w += U(lhs.w); 
		return rhs; 
	} 
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

// Vector-Vector substraction with rvalue 
export template<typename T, typename U> 
requires castable<T, U> && sim_arithm<U> 
inline 
auto operator- (const Vec<T>& lhs, Vec<U>&& rhs) -> Vec<U>& 
{ 
	rhs.x -= U(lhs.x); 
	rhs.y -= U(lhs.y); 
	rhs.z -= U(lhs.z); 
	rhs.w -= U(lhs.w); 
	return -rhs; 
} 

// Vector-Vector substraction with rvalue 
export template<typename T, typename U> 
requires castable<T, U> && sim_arithm<T> 
inline 
auto operator- (Vec<T>&& lhs, const Vec<U>& rhs) -> Vec<T>& 
{ 
	lhs.x -= T(rhs.x); 
	lhs.y -= T(rhs.y); 
	lhs.z -= T(rhs.z); 
	lhs.w -= T(rhs.w); 
	return lhs; 
} 

// Vector-Vector substraction with rvalues 
export template<typename T, typename U> 
requires castable<T, U> && sim_arithm<T> && sim_arithm<U> 
inline 
auto operator- (Vec<T>&& lhs, Vec<U>&& rhs) -> Vec<decltype(lhs.x - rhs.x)> 
{ 
	using W = decltype(lhs.x - rhs.x); 
	if constexpr(std::is_same<W, T>) 
	{ 
		lhs.x -= T(rhs.x); 
		lhs.y -= T(rhs.y); 
		lhs.z -= T(rhs.z); 
		lhs.w -= T(rhs.w); 
		return lhs; 
	} 
	else 
	{ 
		rhs.x -= U(lhs.x); 
		rhs.y -= U(lhs.y); 
		rhs.z -= U(lhs.z); 
		rhs.w -= U(lhs.w); 
		return -rhs; 
	} 
} 

// Vector output operator 
export template<typename T> 
requires stream_ops<T> 
inline 
std::ostream& operator<< (std::ostream& out, const Vec<T>& v) 
{ 
	out << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']'; 
	return out; 
} 

// Vector output operator with rvalue 
export template<typename T> 
requires stream_ops<T> 
inline 
std::ostream& operator<< (std::ostream& out, const Vec<T>&& v) 
{ 
	out << '[' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ']'; 
	return out; 
} 
