module; 

export module polygon; 

import std.core; 

import vec; 
import ray; 
import render_constants; 

export template<typename T = float> 
class Polygon 
{ 
private: 
	bool m_HasNormal{false}; 
	Vec<T> m_A{}, m_B{}, m_C{}, m_V1{}, m_V2{}, m_Normal{}; 
public: 
	Polygon () {}; 
	Polygon (const Vec<T>& a, const Vec<T>& b, const Vec<T>& c) : 
		m_A{a}, m_B{b}, m_C{c} {}; 

	void calc_normal () 
	{ 
		if (!m_HasNormal) 
		{ 
			m_V1 = m_B - m_A; 
			m_V2 = m_C = m_A; 

			m_Normal = (m_V1 ^ m_V2).normalize(); 
			m_HasNormal = true; 
		} 
	} 

	const Vec<T>& get_normal () 
	{ 
		if (!m_HasNormal) 
			calc_normal(); 
		return m_Normal; 
	} 

	Vec<T> get_min_coords() 
	{ 
		T x = std::min(m_A.x, std::min(m_B.x, m_C.x)); 
		T y = std::min(m_A.y, std::min(m_B.y, m_C.y)); 
		T z = std::min(m_A.z, std::min(m_B.z, m_C.z)); 
		return {x, y, z}; 
	} 

	Vec<T> get_max_coords() 
	{ 
		T x = std::max(m_A.x, std::max(m_B.x, m_C.x)); 
		T y = std::max(m_A.y, std::max(m_B.y, m_C.y)); 
		T z = std::max(m_A.z, std::max(m_B.z, m_C.z)); 
		return {x, y, z}; 
	} 

	bool ray_intersect (const Vec<T>& ray, Vec<T>& x_point) 
	{ 
		if (!m_HasNormal) 
			calc_normal(); 

		Vec<T> p_v = ray.dir ^ m_V2; 
		T det = m_V1 * p_v; 

		if constexpr (BF_CULLING) 
		{ 
			if (det < static_cast<T>(EPS)) 
				return false; 
		} 
		else 
			if (std::abs(det) < static_cast<T>(EPS)) 
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
