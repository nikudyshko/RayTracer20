module; 

export module shell; 

import std.core; 

import material; 
import surface; 

export template<typename T = float> 
class Shell 
{ 
private: 
	OpticalBulk<T> m_BulkOpt{}; 
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
	void add_polygon(Surface<T> surf) { m_Mesh.push_back(surf); } 

	void add_polygons(const std::initializer_list< Surface<T> >& mesh) 
	{ 
		std::copy(mesh.begin(), mesh.end(), std::back_inserter(m_Mesh)); 
	} 
	void add_polygons(const std::vector< Surface<T> >& mesh) 
	{ 
		std::copy(mesh.begin(), mesh.end(), std::back_inserter(m_Mesh)); 
	}
}; 
