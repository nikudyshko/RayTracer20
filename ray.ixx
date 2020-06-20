module; 

export module ray; 

import vec; 

export template<typename T = float> 
struct Ray 
{ 
	Vec<T> orig, dir; 

	inline 
	Ray() : orig{}, dir{} {}; 

	inline 
	Ray(const Vec<T>& orig_, const Vec<T>& dir_) : 
		orig{orig_}, dir{dir_} {} 
}; 
