module; 

export module ray; 

import std.core; 

import vec; 
import material; 

// Ray structure 
export template<typename T> 
struct Ray 
{ 
	// Intensity of the light 
	T intensity{}; 
	// Shows if the ray is inner or outer 
	T in_out{T(1)}; 
	// origin - origin of the ray 
	// dir - direction of ray 
	Vec<T> origin{}, dir{}; 

	// Pixel coordinates for Camera ray 
	Vec<T> pc{}; 

	// Color of the pixel 
	Vec<T> color{}; 

	// Default constructor 
	inline 
	Ray () {} 

	// Constructs a Ray from two vectors - origin and direction 
	inline 
	Ray (const Vec<T>& origin_, const Vec<T>& dir_) : 
		origin{origin_}, dir{dir_} {} 
}; 
