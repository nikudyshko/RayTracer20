module; 

export module ray; 

import std.core; 

import vec; 
import material; 

// Struct that describes hit_spot 
export template<typename T> 
struct HitSpot 
{ 
	size_t shell_id{}, surface_id{};  
	T dist{std::numeric_limits<T>::max()}; 
	Vec<T> lx_point{}, gx_point{}, normal{}; 
	OpticalSurface<T> mat{}; 
}; 

// Struct that describes lighting properties 
export template<typename T> 
struct Lighting 
{ 
	std::vector<T> diffuse_lights{}, specular_lights{}; 
}; 

// Ray structure 
export template<typename T> 
struct Ray 
{ 
	// Flag to show, if the Ray hit the surface 
	bool hit{false}; 
	// Intensity of the light 
	T intensity{}; 
	// origin - origin of the ray 
	// dir - direction of ray 
	Vec<T> origin{}, dir{}; 

	// Pixel coordinates for Camera ray 
	Vec<T> pc{}; 

	// Color of the pixel 
	Vec<T> color{}; 

	// Hit spots map. Contains render depth as key value, and a HitSpot struct as a value 
	std::unordered_map< size_t, HitSpot<T> > hit_spots{}; 

	// Lighting map. Contains render depth as key value, and Lighting struct as a value 
	std::unordered_map< size_t, Lighting<T> > lighting{}; 

	// Default constructor 
	inline 
	Ray () {} 

	// Constructs a Ray from two vectors - origin and direction 
	inline 
	Ray (const Vec<T>& origin_, const Vec<T>& dir_) : 
		origin{origin_}, dir{dir_} {} 
}; 
