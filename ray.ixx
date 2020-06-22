module; 

export module ray; 

import vec; 

export template<typename T = float> 
struct Ray 
{ 
	Vec<T> origin{}, dir{}; 

	inline 
	Ray () {} 

	inline 
	Ray (const Vec<T>& origin_, const Vec<T>& dir_) : 
		origin{origin_}, dir{dir_} {} 
}; 
