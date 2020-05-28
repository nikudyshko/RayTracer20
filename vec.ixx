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
{ { std::sqrt(v) } -> T; }; 

template<typename T = float> 
requires default_initializable<T> && sim_arithm<T> && rootable<T> 
{  
	union 
	{ 
		T vec_[4]; 
		struct 
		{ 
			T x, y, z, w; 
		} 
		struct 
		{ 
			T r, g, b, a; 
		}
	} 

	inline 
	Vec() : 
		x{T()}, y{T()}, z{T()}, w{T()} {} 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec(U v) : 
		x{static_cast<T>(v)}, 
		y{static_cast<T>(v)}, 
		z{static_cast<T>(v)}, 
		w{static_cast<T>(v)} {} 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec(U x_, U y_, U z_, U w_ = U()) : 
		x{static_cast<T>(x_)}, 
		y{static_cast<T>(y_)}, 
		z{static_cast<T>(z_)}, 
		w{static_cast<T>(w_)} {} 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec(const Vec<U>& v) : 
		x{static_cast<T>(v.x)}, 
		y{static_cast<T>(v.y)}, 
		z{static_cast<T>(v.z)}, 
		w{static_cast<T>(v.w)} {} 

	inline 
	T& operator[](const size_t i) 
	{ 
		assert(i < 4); 
		return vec_[i]; 
	} 

	inline 
	const T& operator[](const size_t i) 
	{ 
		assert(i < 4); 
		return vec_[i]; 
	} 

	inline 
	T length() { return std::sqrt(x*x + y*y + z*z + w*w); } 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec<T> normalize(U scale = static_cast<U>(1)) 
	{ 
		T s = static_cast<T>(scale)/length(); 
		return Vec<T>(x/s, y/s, z/s, w/s); 
	} 

	template<typename U = T> 
	requires castable<T, U> 
	inline 
	Vec<T>& operator=(const Vec<U>& rhs) 
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
	Vec<T>& operator+=(const Vec<U>& rhs) 
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
	Vec<T>& operator-=(const Vec<U>& rhs) 
	{ 
		x -= static_cast<T>(rhs.x); 
		y -= static_cast<T>(rhs.y); 
		z -= static_cast<T>(rhs.z); 
		w -= static_cast<T>(rhs.w); 
		return *this; 
	} 
}; 

template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator*(const Vec<T>& lhs, const Vec<U>& rhs) -> decltype(lhs.x*rhs.x) 
{ 
	return lhs.x*rhs.x + lhs.y*rhs.y + 
		   lhs.z*rhs.z + lhs.w*rhs.w; 
} 

template<typename T, typename U> 
requires mut_arithm<T, U> && 
		 requires(T t, U u) default_initializable<decltype(t*u)> 
inline 
auto operator^(const Vec<T>& lhs, const Vec<U>& rhs) -> Vec<decltype(lhs.x*rhs.y)> 
{ 
	using V = decltype(lhs.x*rhs.y); 
	V x = lhs.y*rhs.z - lhs.z*rhs.y; 
	V y = lhs.z*rhs.x - lhs.x*rhs.z; 
	V z = lhs.z*rhs.y - lhs.y*rhs.x; 
	V w = V(); 
	return Vec<V>(x, y, z, w); 
} 

template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator+(const Vec<T>& lhs, const Vec<U>& rhs) -> Vec<decltype(lhs.x + rhs.x)> 
{ 
	using V = decltype(lhs.x + rhs.x); 
	return Vec<V>(lhs.x + rhs.x, lhs.y + rhs.y, 
				  lhs.z + rhs.z, lhs.w + rhs.w); 
} 

template<typename T, typename U> 
requires mut_arithm<T, U> 
inline 
auto operator-(const Vec<T>& lhs, const Vec<U>& rhs) -> Vec<decltype(lhs.x - rhs.x)> 
{ 
	using V = decltype(lhs.x - rhs.x); 
	return Vec<V>(lhs.x - rhs.x, lhs.y - rhs.y, 
				  lhs.z - rhs.z, lhs.w - rhs.w); 
} 
