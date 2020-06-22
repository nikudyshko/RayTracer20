module; 

export module camera; 

import std.core; 

import vec; 
import ray; 

export template<typename T = float> 
class Camera 
{ 
private: 
	bool m_HasRays{false}; 
	size_t m_Width{}, m_Height{}; 
	size_t m_CurrentRay{}; 
	T m_FOV{}; 
	T m_AspectRatio{}; 
	Vec<T> m_Origin{}, m_Dir{}; 
	std::vector< Ray<T> > m_Rays; 
public: 
	inline 
	Camera() = delete; 

	inline 
	Camera(size_t width, 
		   size_t height, 
		   T FOV, 
		   const Vec<T>& origin, 
		   const Vec<T>& dir) : 
		m_Width{width}, 
		m_Height{height}, 
		m_FOV{FOV}, 
		m_AspectRatio{m_Width/static_cast<T>(m_Height)}, 
		m_Origin{origin}, 
		m_Dir{dir} {} 

	void set_resolution(size_t width, size_t height) 
	{ 
		m_Width = width; 
		m_Height = height; 

		m_AspectRatio = m_Width/static_cast<T>(m_Height); 

		calc_rays(); 
	} 

	void set_FOV(T FOV) 
	{ 
		m_FOV = FOV; 

		calc_rays(); 
	} 

	void set_origin(const Vec<T>& origin) 
	{ 
		m_Origin = origin; 

		calc_rays(); 
	} 

	void set_direction(const Vec<T>& dir) 
	{ 
		m_Dir = dir; 

		calc_rays(); 
	} 

	void calc_rays() 
	{ 
		m_Rays.resize(m_Width*m_Height); 

		for (size_t i = 0; i < m_Width; ++i) 
			for (size_t j = 0; j < m_Height; ++j) 
			{ 
				T dir_x = (2*(i + 0.5)/static_cast<T>(m_Width) - 1)*std::tan(m_FOV/2.)*m_AspectRatio; 
				T dir_y = -(2*(j + 0.5)/static_cast<T>(m_Height) - 1)*std::tan(m_FOV/2.); 
				m_Rays[i*m_Height + j] = { .x = dir_x, .y = dir_y, .z = static_cast<T>(-1) }.normalize(); 
			} 

		m_HasRays = true; 
	} 

	const Vec<T>& get_next_ray() const 
	{ 
		if (m_HasRays) 
		{ 
			m_CurrentRay = (m_CurrentRay + 1) % m_Rays.size(); 

			return m_Rays[m_CurrentRay]; 
		} 
	} 
}; 
