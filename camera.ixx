module; 

export module camera; 

import std.core; 

import vec; 
import ray; 

// Structure to implement a Camera 
export template<typename T = float> 
class Camera 
{ 
private: 
	// Shows if the camera Rays has been generated 
	bool m_HasRays{false}; 
	// Width and Height of Camera (pixels) 
	size_t m_Width{}, m_Height{}; 
	// Index of the current Ray 
	size_t m_CurrentRay{}; 
	// Cameras' fiels of view 
	T m_FOV{}; 
	// Cameras' aspect ratio 
	T m_AspectRatio{}; 
	// Cameras' origin and direction 
	Vec<T> m_Origin{}, m_Dir{}; 
	// Array of Rays 
	std::vector< Ray<T> > m_Rays; 
public: 
	// Default constructor 
	inline 
	Camera() = delete; 

	// Construct a Camera with width, height, 
	// FOV and origin, direction parameters  
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

	// Function to set width and height of camera 
	void set_resolution(size_t width, size_t height) 
	{ 
		m_Width = width; 
		m_Height = height; 

		m_AspectRatio = m_Width/static_cast<T>(m_Height); 

		calc_rays(); 
	} 

	// Function to set field of view 
	void set_FOV(T FOV) 
	{ 
		m_FOV = FOV; 

		calc_rays(); 
	} 

	// Function to set origin of Camera 
	void set_origin(const Vec<T>& origin) 
	{ 
		m_Origin = origin; 

		calc_rays(); 
	} 

	// Function to set direction of Camera 
	void set_direction(const Vec<T>& dir) 
	{ 
		m_Dir = dir; 

		calc_rays(); 
	} 

	// Function to calculate Rays 
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

	// Function return a single Ray 
	const Vec<T>& get_next_ray() const 
	{ 
		if (m_HasRays) 
		{ 
			m_CurrentRay = (m_CurrentRay + 1) % m_Rays.size(); 

			return m_Rays[m_CurrentRay]; 
		} 
	} 

	// Function to return a whole array of Rays 
	const std::vector< Vec<T> >& get_rays() 
	{ 
		if (m_HasRays) 
			return m_Rays; 
	} 
}; 
