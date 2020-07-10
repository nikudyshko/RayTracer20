module; 

export module polygon; 

import std.core; 

import vec; 
import ray; 
import render_constants; 

// Struct to implement Polygon 
export template<typename T> 
class Polygon 
{ 
private: 
	// Shows if the normal has been calculated before 
	bool m_HasNormal{false}; 
	// Vector that stores polygon vertices, polygon edges and its' normal 
	Vec<T> m_A{}, m_B{}, m_C{}, m_V1{}, m_V2{}, m_Normal{}; 
public: 
	// Default constructor 
	Polygon () {}; 
	// Contstructs polygon from three vertices 
	Polygon (const Vec<T>& a, const Vec<T>& b, const Vec<T>& c) : 
		m_A{a}, m_B{b}, m_C{c} {}; 

	// Calculates normal of the polygon 
	void calc_normal () 
	{ 
		if (!m_HasNormal) 
		{ 
			m_V1 = m_B - m_A; 
			m_V2 = m_C - m_A; 

			m_Normal = (m_V1 ^ m_V2).normalize(); 
			m_HasNormal = true; 
		} 
	} 

	// Returns the normal vector of polygon 
	const Vec<T>& get_normal () 
	{ 
		if (!m_HasNormal) 
			calc_normal(); 
		return m_Normal; 
	} 

	// Returns the coordinates of Polygon 
	std::vector< Vec<T> > get_coords() const 
	{ return { m_A, m_B, m_C }; } 

	// Code to check if the ray intersects the Polygon, Moller-Trumbore algorithm 
	bool ray_intersect (const Vec<T>& ray, Vec<T>& x_point) const 
	{ 
		if (!m_HasNormal) 
			calc_normal(); 

		Vec<T> p_v = ray.dir ^ m_V2; 
		T det = m_V1 * p_v; 

		if constexpr (BF_CULLING) 
		{ 
			if (det < T(EPS)) 
				return false; 
		} 
		else 
			if (std::abs(det) < T(EPS)) 
				return false; 
			else 
			{ 
				T inv_det = 1 / det; 

				Vec<T> t_v = ray.origin - m_A; 
				x_point.u = inv_det * t_v * p_v; 
				if (x_point.u < 0 || x_point.u > 1) 
					return false; 

				Vec<T> q_v = t_v ^ m_V1; 
				x_point.v = inv_det * ray.dir * q_v; 

				if (x_point.v < 0 || x_point.u + x_point.v > 1) 
					return false; 

				x_point.t = inv_det * m_V2 * q_v; 

				return true; 
			} 
	} 
}; 
