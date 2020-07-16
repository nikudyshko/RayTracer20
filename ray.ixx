module; 

export module ray; 

import vec; 

// Ray structure 
export template<typename T> 
struct Ray 
{ 
	// Intensity of the light 
	T intensity{}; 
	// Color of the light 
	Vec<T> color{};  
	// origin - origin of the ray 
	// dir - direction of ray 
	Vec<T> origin{}, dir{}; 

	// Pixel coordinates for Camera ray 
	Vec<T> pc{};  

	// Default constructor 
	inline 
	Ray () {} 

	// Constructs a Ray from two vectors - origin and direction 
	inline 
	Ray (const Vec<T>& origin_, const Vec<T>& dir_) : 
		origin{origin_}, dir{dir_} {} 
}; 
