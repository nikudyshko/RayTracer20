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
export template<typename T> 
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
		Vec<T> center_point{}, dist_point{}; 
		for (Surface<T>& s : m_Mesh) 
		{ 
			auto[p1, p2, p3] = s.get_polygon().get_coords(); 
			center_point += (p1 + p2 + p3); 
		} 

		T s = 1/T(m_Mesh.size()); 
		center_point.x *= s; 
		center_point.y *= s; 
		center_point.z *= s; 

		T dist = T{}; 
		for (Surface<T>& s : m_Mesh) 
		{ 
			auto[p1, p2, p3] = s.get_polygon().get_coords(); 

			if ((p1 - center_point).length() > dist) 
				dist = (p1 - center_point).length(); 
			if ((p2 - center_point).length() > dist) 
				dist = (p2 - center_point).length(); 
			if ((p3 - center_point).length() > dist) 
				dist = (p3 - center_point).length(); 
		} 

		m_BoundRadius = dist; 
		m_BoundOrigin = center_point; 

		std::cout << m_BoundRadius << '\n' << m_BoundOrigin << '\n'; 
	} 
}; 
