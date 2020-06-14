module; 

export module ray; 

import vec; 

template<typename T = float> 
class Ray 
{ 
public:  
	Vec<T> orig, dir; 

	Ray() = delete; 
	Ray(const Vec<T>& orig_, const Vec<T>& dir_) : 
		orig{orig_}, dir{dir_} {} 
}; 
