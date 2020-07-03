module; 

export module ray; 

import vec; 

// Ray structure 
export template<typename T> 
struct Ray 
{ 
	// origin - origin of the ray 
	// dir - direction of ray 
	Vec<T> origin{}, dir{}; 

	// Default constructor 
	inline 
	Ray () {} 

	// Constructs a Ray from two vectors - origin and direction 
	inline 
	Ray (const Vec<T>& origin_, const Vec<T>& dir_) : 
		origin{origin_}, dir{dir_} {} 
}; 
