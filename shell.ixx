module; 

export module shell; 

import std.core; 

import vec; 
import material; 
import surface; 

export template<typename T = float> 
class Shell 
{ 
private: 
	OpticalBulk<T> m_BulkOpt{}; 

	Vec<T> m_BoundOrigin{}, m_BoundRadius{}; 

	std::vector< Surface<T> > m_Mesh{}; 

	std::vector< Vec<T> > m_TestRays{}; 

	std::vector< Shell<T> > m_InnerShels{}; 
public: 
	Shell() {} 
	Shell(const OpticalBulk<T>& bulk_opt) : 
		m_BulkOpt{bulk_opt}, m_Mesh{} {} 
	Shell(std::initializer_list< Surface<T> > mesh) : 
		m_BulkOpt{}, m_Mesh(mesh) {}; 

	void set_opt_prop(const OpticalBulk<T>& bulk_opt) { m_BulkOpt = OpticalBulk<T>(bulk_opt); } 
	void add_surface(Surface<T> surf) { m_Mesh.push_back(surf); } 
	template< template<typename, typename> typename Container > 
	void add_surfaces(const Container< Surface<T>, std::allocator<Surface<T>> >& mesh) 
	{ 
		std::copy(mesh.begin(), mesh.end(), std::back_inserter(m_Mesh)); 
	} 

	void calc_bound_sphere() 
	{ 
		Surface<T> min_point, max_point; 
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
		m_BoundOrigin = (max_point + min_point)/2; 
		m_BoundRadius = (max_point - min_point)/2; 
	} 
}; 
