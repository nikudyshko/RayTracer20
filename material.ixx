module; 

export module material; 

import vec; 

// Optical properties of surface structure 
export template<typename T> 
struct OpticalSurface 
{ 
	// Specular exponent parameter 
	T specular{}; 
	// reflection - reflective properties of surface 
	// color - surface color 
	Vec<T> reflection{}, color{}; 

	// Default constructor 
	OpticalSurface() {} 
	// Constructs properties from specular, reflection and color parameters 
	OpticalSurface(T specular_, 
				   const Vec<T>& reflection_, 
				   const Vec<T>& color_) : 
		specular{specular_}, 
		reflection{reflection_}, 
		color{color_} {} 
}; 

// Optical properties of bulk structure 
export template<typename T> 
struct OpticalBulk 
{ 
	// refraction - refraction parameter 
	// transparency - transparency parameter 
	T refraction{}, transparency{}; 

	// Default constructor 
	OpticalBulk() {} 
	// Constructs properties from refraction and transparency parameters 
	OpticalBulk(T refraction_, 
				T transparency_) : 
		refraction{refraction_}, 
		transparency{transparency_} {} 
}; 
