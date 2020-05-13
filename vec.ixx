export module vec; 

template<typename T = float> 
export struct Vec 
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

	inline Vec() : 
		x{T()}, y{T()}, z{T()}, w{T()} {} 

	inline Vec(T v) : 
		x{v}, y{v}, z{v}, w{v} {} 

	inline Vec(T x_, T y_, T z_, T w_) : 
		x{x_}, y{y_}, z{z_}, w{w_} {} 

	inline Vec(const Vec<T>& v) : 
		x{v.x}, y{v.y}, z{v.z}, w{v.w} {} 

	inline T& operator[](const size_t i) 
	{ 
		assert(i < 4); 
		return vec_[i]; 
	} 

	inline const T& operator[](const size_t i) 
	{ 
		assert(i < 4); 
		return vec_[i]; 
	} 

	inline T length() { return std::sqrt(x*x + y*y + z*z + w*w); } 

	inline Vec<T>& normalize(T scale = static_cast<T>(1)) 
	{ 
		*this = scale*(*this)/length(); 
		return *this; 
	} 

	inline Vec<T>& operator=(const Vec<T>& rhs) 
	{ 
		x = rhs.x; 
		y = rhs.y; 
		z = rhs.z; 
		w = rhs.w; 
		return *this; 
	} 

	inline Vec<T>& operator+=(const Vec<T>& rhs) 
	{ 
		x += rhs.x; 
		y += rhs.y; 
		z += rhs.z; 
		w += rhs.w; 
		return *this; 
	} 

	inline Vec<T>& operator-=(const Vec<T>& rhs) 
	{ 
		x -= rhs.x; 
		y -= rhs.y; 
		z -= rhs.z; 
		w -= rhs.w; 
		return *this; 
	} 
}; 

template<typename T> 
inline T operator*(const Vec<T>& lhs, const Vec<T>& rhs) 
{ 
	return lhs.x*rhs.x + lhs.y*lhs.y 
		   lhs.z*rhs.z + lhs.w*rhs.w; 
} 

template<typename T> 
inline T operator+(const Vec<T>& lhs, const Vec<T>& rhs) 
{ 

}
