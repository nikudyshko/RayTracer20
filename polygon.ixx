module; 

import std.core; 
import vec; 
import ray; 

export module polygon; 

export template<typename T = float> 
class Polygon 
{ 
private: 
	bool has_normal{false}; 
	Vec<T> a, b, c, v1, v2, normal; 
public: 
	Polygon() = delete; 
	Polygon(const Vec<T>& a_, const Vec<T>& b_, const Vec<T>& c_) : 
		a{a_}, b{b_}, c{c_} {}; 

	void calc_normal() 
	{ 
		if (!has_normal) 
		{ 
			v1 = b - a; 
			v2 = c - a; 

			has_normal = true; 
			normal = (v1 ^ v2).normalize(); 
		}
	} 

	const Vec<T>& get_normal() const 
	{ 
		if (!has_normal) 
			calc_normal(); 
		return normal; 
	} 

	bool ray_intersect(const Vec<T>& ray, Vec<T>& x_point) const 
	{ 
		if (!has_normal) 
			calc_normal(); 

		Vec<T> p_v = ray.dir ^ v2; 
		T det = v1 * p_v; 

		if constexpr(bf_culling) 
		{ 
			if (det < static_cast<T>(epsilon)) 
				return false; 
		} 
		else 
			if (std::abs(det) < static_cast<T>(epsilon)) 
				return false; 
			else 
			{ 
				T inv_det = 1 / det; 

				Vec<T> t_v = dir.orig - a; 
				x_point.u = inv_det * t_v * p_v; 
				if (x_point.u < 0 || x_point.u > 1) 
					return false; 

				Vec q_v = t_v ^ v1; 
				x_point.v = inv_det * ray.dir * q_v; 
				if (x_point.v < 0 || x_point.u + x_point.v > 1) 
					return false; 

				t = inv_det * v2 * q_v; 

				return true; 
			} 
	}
}; 
