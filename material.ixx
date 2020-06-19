module; 

export module material; 

import vec; 

export template<typename T = float> 
struct OpticalSurface 
{ 
	T specular{}; 
	Vec<T> reflection{}, color{}; 

	OpticalSurface() {} 
	OpticalSurface(T specular_, 
				   const Vec<T>& reflection_, 
				   const Vec<T>& color_) : 
		specular{specular_}, 
		reflection{reflection_}, 
		color{color_} {} 
}; 

export template<typename T = float> 
struct OpticalBulk 
{ 
	T refraction{}, transparency{}; 

	OpticalBulk() {} 
	OpticalBulk(T refraction_, 
				T transparency_) : 
		refraction{refraction_}, 
		transparency{transparency_} {} 
}; 
