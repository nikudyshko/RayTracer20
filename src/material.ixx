module; 

export module material; 

import vec; 

// Surface attribute: metalic or dielectric 
enum class Conduct 
{ 
	METAL, 
	DIELECTRIC 
}; 

// Type of optical calculations 
enum class OptCalc 
{ 
	DIFFUSE, 
	REFLECT, 
	REFLECT_AND_REFRACT 
}; 

// Optical properties of surface structure 
export template<typename T> 
struct OpticalSurface 
{ 
	Conduct c_type{}; 
	OptCalc oc_type{}; 
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

	// Bulk color 
	Vec<T> color{}; 

	// Default constructor 
	OpticalBulk() {} 
	// Constructs properties from refraction and transparency parameters 
	OpticalBulk(T refraction_, 
				T transparency_) : 
		refraction{refraction_}, 
		transparency{transparency_} {} 
}; 
