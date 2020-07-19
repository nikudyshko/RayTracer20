module; 

export module ray; 

import std.core; 

import vec; 

// Ray structure 
export template<typename T> 
struct Ray 
{ 
	// Intensity of the light 
	T intensity{}; 
	// origin - origin of the ray 
	// dir - direction of ray 
	Vec<T> origin{}, dir{}; 

	// Pixel coordinates for Camera ray 
	Vec<T> pc{}; 

	// Hit spots map, contains distance (key), local coordinates, global coordinates and color  
	// Automatically sorts via keys (distance) 
	std::map< T, std::tuple<Vec<T>, Vec<T>, Vec<T>, Vec<T>> > hit_spots{}; 

	// Default constructor 
	inline 
	Ray () {} 

	// Constructs a Ray from two vectors - origin and direction 
	inline 
	Ray (const Vec<T>& origin_, const Vec<T>& dir_) : 
		origin{origin_}, dir{dir_} {} 
}; 
