module; 

export module material; 

import vec; 

export template<typename T = float> 
struct Material 
{ 
	T specular{}, refraction{}; 
	Vec<T> reflection{}, color{}; 

	Material() {}; 
	Material(T refraction_, 
			 T specular_, 
			 const Vec<T>& reflection_, 
			 const Vec<T>& color_) : 
		refraction{refraction_}, 
		specular{specular_}, 
		reflection{reflection_}, 
		color{color_} {} 
}; 
