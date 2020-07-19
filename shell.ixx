module; 

#include <cassert> 

export module shell; 

import std.core; 

import vec; 
import ray; 
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
	// Flag to check if the Shell has mesh 
	bool m_HasMesh{false}; 
	// Flag to check if the Shell has Bound Sphere 
	bool m_HasBoundSphere{false}; 
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
		m_BulkOpt{}, m_Mesh{mesh}, m_HasMesh{true} {} 
	// Constructs a Shell from other Shells 
	Shell(std::initializer_list< Shell<T> > inner_shells) : 
		m_BulkOpt{}, m_Mesh{}, m_InnerShells{inner_shells} {} 

	// Function to set optical properties 
	void set_opt_prop(const OpticalBulk<T>& bulk_opt) { m_BulkOpt = OpticalBulk<T>(bulk_opt); } 
	// Function to add a single Surface to mesh 
	void add_surface(Surface<T> surf) 
	{ 
		m_Mesh.push_back(surf); 
		m_HasMesh = true; 
	} 
	// Function to set a complete mesh 
	template<typename Container> 
	void add_surfaces(const Container& mesh) 
	{ 
		assert((std::is_same<Container::value_type, Surface<T>>::value)); 
		m_Mesh.clear(); 
		std::copy(mesh.begin(), mesh.end(), std::back_inserter(m_Mesh)); 
		m_HasMesh = true; 
	} 

	// Returns a vector of inner Shells 
	const std::vector< Shell<T> >& get_inner_shells() const 
	{ return m_InnerShells; } 

	// Sets the bound sphere 
	void set_bound_sphere(T bound_radius, const Vec<T>& bound_origin) 
	{ 
		m_BoundRadius = bound_radius; 
		m_BoundOrigin = bound_origin; 
		m_HasBoundSphere = true; 
	}

	// Function to calculate bound sphere 
	void calc_bound_sphere() 
	{ 
		assert(m_HasMesh); 
		Vec<T> center_point{}; 
		for (Surface<T>& s : m_Mesh) 
		{ 
			std::vector< Vec<T> > ps = s.get_polygon().get_coords(); 

			for (Vec<T>& p : ps) 
				center_point += p; 
		} 

		T s = 1/T(m_Mesh.size()); 
		center_point *= s; 

		T dist = T{}; 
		for (Surface<T>& s : m_Mesh) 
		{ 
			std::vector< Vec<T> > ps = s.get_polygon().get_coords(); 

			for (Vec<T>& p : ps) 
			{ 
				if (T l = (p - center_point).length(); l > dist) 
					dist = l; 
			}
		} 

		m_BoundRadius = dist; 
		m_BoundOrigin = center_point; 

		m_HasBoundSphere = true; 
	} 

	// Calculates ray-bound sphere intersection 
	bool hit_sphere(const Ray<T>& ray) const 
	{ 
		assert(m_HasBoundSphere); 
		Vec<T> L = m_BoundOrigin - ray.origin; 

		T a = ray.dir*ray.dir; 
		T b = T(2)*ray.dir*L; 
		T c = L*L - m_BoundRadius*m_BoundRadius; 

		if (T d = b*b - T(4)*a*c; d > T(0)) 
		{ 
			T sd = std::sqrt(d); 
			T t1 = (-b - sd)/(2*a); 
			T t2 = (-b + sd)/(2*a); 

			if ((t1 >= T(0)) || (t2 >= T(0))) 
				return true; 
		} 

		return false; 
	} 

	// Traces the Ray 
	bool trace(Ray<T>& r) 
	{ 
		assert(m_HasMesh); 
		T dist{}; 
		Vec<T> lx_point{}, gx_point{}; 
		for (const Surface<T>& s : m_Mesh) 
			if (s.get_polygon().ray_intersect(r, dist, lx_point, gx_point)) 
			{ 
				r.hit_spots[dist] = std::make_tuple(lx_point, gx_point, s.get_polygon().get_normal(), s.get_color(lx_point)); 
				return true; 
			} 
		return false; 
	}
}; 
