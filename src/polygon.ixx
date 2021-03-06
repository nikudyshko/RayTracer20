module; 

#include <cassert> 

export module polygon; 

import std.core; 

import vec; 
import mat; 
import ray; 
import geometry; 
import render_constants; 

// Struct to implement Polygon 
export template<typename T> 
class Polygon : public Geometry<T> 
{ 
private: 
	// Shows if the normal has been calculated before 
	bool m_HasNormal{false}; 
	// Vector that stores polygon vertices, polygon edges and its' normal 
	Vec<T> m_A{}, m_B{}, m_C{}, m_V1{}, m_V2{}, m_Normal{}; 

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
public: 
	// Default constructor 
	Polygon () {}; 
	// Contstructs polygon from three vertices 
	Polygon (const Vec<T>& a, const Vec<T>& b, const Vec<T>& c) : 
		m_A{a}, m_B{b}, m_C{c} { calc_normal(); }; 

	Geometry<T>* get_copy() const override 
	{ 
		return new Polygon{m_A, m_B, m_C}; 
	}

	// Apply a matrix transform to every point of Polygon 
	void transform(const Mat<T>& matrix) override 
	{ 
		m_A.w = T(1); 
		m_A = matrix*m_A; 
		m_A.w = T(0); 
		m_B.w = T(1); 
		m_B = matrix*m_B; 
		m_B.w = T(0); 
		m_C.w = T(1); 
		m_C = matrix*m_C; 
		m_C.w = T(0); 

		m_HasNormal = false; 
		calc_normal(); 
	} 

	// Returns the normal vector of polygon 
	Vec<T> get_normal(const Vec<T> &point) const override 
	{ 
		assert(m_HasNormal); 
		return m_Normal; 
	} 

	// Returns the coordinates of Polygon 
	std::vector< Vec<T> > get_coords() const override 
	{ return { m_A, m_B, m_C }; } 

	// Code to check if the Ray intersects the Polygon, Moller-Trumbore algortihm 
	// ray - ray to check, sq_dist - squared distance to hit point, lx_point - local coordinates of hit point, 
	// gx_point - global coordinates of hit point 
	bool ray_intersect (const Ray<T>& ray, T& dist, Vec<T>& lx_point, Vec<T>& gx_point) const override 
	{ 
		assert(m_HasNormal); 

		Vec<T> p_v = ray.dir ^ m_V2; 
		T det = m_V1 * p_v; 

		if constexpr (BF_CULLING) 
		{ 
			if (det < T(EPS)) 
				return false; 
		} 

		if (std::abs(det) < T(EPS)) 
			return false; 
		else 
		{ 
			T inv_det = 1 / det; 

			Vec<T> t_v = ray.origin - m_A; 
			lx_point.u = inv_det * t_v * p_v; 
			if (lx_point.u < 0 || lx_point.u > 1) 
				return false; 

			Vec<T> q_v = t_v ^ m_V1; 
			lx_point.v = inv_det * ray.dir * q_v; 

			if (lx_point.v < 0 || lx_point.u + lx_point.v > 1) 
				return false; 

			lx_point.t = inv_det * m_V2 * q_v; 

			gx_point = ray.origin + lx_point.t*ray.dir; 

			dist = lx_point.t; 

			return true; 
		} 
	} 
}; 
