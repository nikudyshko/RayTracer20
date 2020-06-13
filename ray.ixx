module; 

import vec; 

export module ray; 

template<typename T = float> 
class Ray 
{ 
public:  
	Vec<T> orig, dir; 

	Ray() = delete; 
	Ray(const Vec<T>& orig_, const Vec<T>& dir_) : 
		orig{orig_}, dir{dir_} {} 
}; 
