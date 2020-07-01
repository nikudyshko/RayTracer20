module; 

export module shell; 

import std.core; 

import vec; 
import material; 
import surface; 

// Structure to implement a shell. Shell represents 
// a basic object - has surface (array of Surface's), 
// inner optical properties (OpticalBulk) and 
// contains another shells 
export template<typename T = float> 
class Shell 
{ 
private: 
	// Bound Sphere radius 
	T m_BoundRadius{}; 

	// Inner optical properties 
	OpticalBulk<T> m_BulkOpt{}; 

	// Bound Sphere origin 
	Vec<T> m_BoundOrigin{}; 

	// Mesh. Represents geometry of Shell  
	std::vector< Surface<T> > m_Mesh{}; 

	// Rays, that hit Bound Sphere. Should be 
	// properly traced against every Surface element 

	std::vector< Vec<T> > m_TestRays{}; 

	// Inner Shells  
	std::vector< Shell<T> > m_InnerShells{}; 
public: 
	// Default constructor 
	Shell() {} 
	// Constructs a Shell from only optical properties 
	Shell(const OpticalBulk<T>& bulk_opt) : 
		m_BulkOpt{bulk_opt}, m_Mesh{} {} 
	// Constructs a Shell from only mesh 
	Shell(std::initializer_list< Surface<T> > mesh) : 
		m_BulkOpt{}, m_Mesh(mesh) {}; 

	// Function to set optical properties 
	void set_opt_prop(const OpticalBulk<T>& bulk_opt) { m_BulkOpt = OpticalBulk<T>(bulk_opt); } 
	// Function to add a single Surface to mesh 
	void add_surface(Surface<T> surf) { m_Mesh.push_back(surf); } 
	// Function to set a complete mesh 
	template<typename Container > 
	void add_surfaces(const Container& mesh) 
	{ 
		std::copy(mesh.begin(), mesh.end(), std::back_inserter(m_Mesh)); 
	} 

	// Function to calculate bound sphere 
	void calc_bound_sphere() 
	{ 
		Vec<T> min_point, max_point; 
		for (Surface<T>& s : m_Mesh) 
		{ 
			Vec<T> temp_min = s.get_min_coords(); 
			Vec<T> temp_max = s.get_max_coords(); 

			min_point.x = std::min(min_point.x, temp_min.x); 
			min_point.y = std::min(min_point.y, temp_min.y); 
			min_point.z = std::min(min_point.z, temp_min.z); 

			max_point.x = std::max(max_point.x, temp_max.x); 
			max_point.y = std::max(max_point.y, temp_max.y); 
			max_point.z = std::max(max_point.z, temp_max.z); 
		} 
		m_BoundOrigin = 0.5*(max_point + min_point);  
		m_BoundRadius = 0.5*(max_point - min_point).length(); 
	} 
}; 
