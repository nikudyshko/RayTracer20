module; 

#include <cassert> 

export module shell; 

import std.core; 

import vec; 
import ray; 
import light; 
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
	// ID given by Renderer 
	size_t m_ShellID{0}; 
	// Bound Sphere radius 
	T m_BoundRadius{}; 

	// Inner optical properties 
	OpticalBulk<T> m_BulkOpt{}; 

	// Bound Sphere origin 
	Vec<T> m_BoundOrigin{}; 

	// Mesh. Represents geometry of Shell  
	std::vector< Surface<T> > m_Mesh{}; 
public: 
	// Default constructor 
	Shell() {} 
	// Constructs a Shell from only optical properties 
	Shell(const OpticalBulk<T>& bulk_opt) : 
		m_BulkOpt{bulk_opt}, m_Mesh{} {} 
	// Constructs a Shell from only mesh 
	Shell(std::initializer_list< Surface<T> > mesh) : 
		m_BulkOpt{}, m_Mesh{mesh}, m_HasMesh{true} 
	{
		for (size_t i = 0; i < m_Mesh.size(); ++i) 
			m_Mesh[i].set_shell_id(i); 
	} 

	inline 
	void set_shell_id(size_t id) noexcept 
	{ m_ShellID = id; } 

	inline 
	size_t get_shell_id() const noexcept 
	{ return m_ShellID; } 

	// Function to set optical properties 
	void set_opt_prop(const OpticalBulk<T>& bulk_opt) { m_BulkOpt = OpticalBulk<T>(bulk_opt); } 
	// Function to add a single Surface to mesh 
	void add_surface(Surface<T> surf) 
	{ 
		m_Mesh.push_back(surf); 
		m_Mesh.back().set_shell_id(m_Mesh.size() - 1); 
		m_HasMesh = true; 
	} 
	// Function to set a complete mesh 
	template<typename Container> 
	void add_surfaces(const Container& mesh) 
	{ 
		assert((std::is_same<Container::value_type, Surface<T>>::value)); 
		m_Mesh.clear(); 
		std::copy(mesh.begin(), mesh.end(), std::back_inserter(m_Mesh)); 
		for (size_t i = 0; i < m_Mesh.size(); ++i) 
			m_Mesh[i].set_surface_id(i); 
		m_HasMesh = true; 
	} 

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

	// Traces the path of Ray 
	bool path_trace(size_t depth, Ray<T>& r) 
	{ 
		assert(m_HasMesh); 
		bool hit{false}; 
		T sq_dist{T(0)}, sq_max_dist{r.hit_spots[depth].sq_dist}; 
		Vec<T> lx_point{}, gx_point{}; 
		for (const Surface<T>& s : m_Mesh) 
			if (s.get_polygon().ray_intersect(r, sq_dist, lx_point, gx_point)) 
			{ 
				if (sq_dist < sq_max_dist) 
				{ 
					hit = true; 
					sq_max_dist = sq_dist; 
					r.hit = hit; 
					r.hit_spots[depth] = {m_ShellID, s.get_surface_id(), sq_dist, lx_point, gx_point, s.get_polygon().get_normal(), s.get_surf_opt(lx_point)}; 
				}
			} 
		return hit;  
	} 

	// Checks if the ray intersects polygons of the shell 
	bool shadow_trace( 
		size_t shell_id, 
		size_t surface_id, 
		T light_distance, 
		const Ray<T>& shadow_ray, 
		const Light<T>& light 
	) const 
	{ 
		assert(m_HasMesh); 

		T dist{}; 
		Vec<T> lx_temp{}, gx_temp{};

		for (const Surface<T>& s : m_Mesh) 
		{ 
			if (!((m_ShellID == shell_id) && (s.get_surface_id() == surface_id)) && 
				s.get_polygon().ray_intersect(shadow_ray, dist, lx_temp, gx_temp) && 
				((light.position - gx_temp).sq_norm() < light_distance)) 
				return true; 
		} 

		return false; 
	}
}; 
