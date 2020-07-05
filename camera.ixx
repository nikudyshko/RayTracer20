module; 

export module camera; 

import std.core; 

import vec; 
import mat; 
import ray; 

// Structure to implement a Camera 
export template<typename T> 
class Camera 
{ 
private: 
	// Shows if the camera has calculated camera-to-world Matrix 
	bool m_HasMatrix{false}; 
	// Shows if the camera Rays has been generated 
	bool m_HasRays{false}; 
	// Width and Height of Camera (pixels) 
	size_t m_Width{}, m_Height{}; 
	// Index of the current Ray 
	mutable size_t m_CurrentRay{}; 
	// Cameras' fiels of view 
	T m_FOV{}; 
	// Cameras' aspect ratio 
	T m_AspectRatio{}; 
	// Cameras' origin and point to look to 
	Vec<T> m_Origin{}, m_LookAt{}; 
	// CameraToWorld matrix 
	Mat<T> m_CTWMatrix{};  
	// Array of Rays 
	std::vector< Ray<T> > m_Rays{}; 
public: 
	// Default constructor 
	inline 
	Camera() = delete; 

	// COnstruct a Camera width width, height, 
	// FOV and origin, look-at parameters 
	inline 
	Camera(size_t width, 
		   size_t height, 
		   T FOV, 
		   const Vec<T>& origin, 
		   const Vec<T>& look_at) : 
		m_Width{width}, 
		m_Height{height}, 
		m_FOV{FOV}, 
		m_AspectRatio{m_Width/T(m_Height)}, 
		m_Origin{origin}, 
		m_LookAt{look_at} {} 

	// Function to set width and height of camera 
	void set_resolution(size_t width, size_t height) 
	{ 
		m_Width = width; 
		m_Height = height; 

		m_AspectRatio = m_Width/T(m_Height); 

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

	// Function to set Cameras' look-at point 
	void set_look_at(const Vec<T>& look_at) 
	{ 
		m_LookAt = look_at; 

		calc_rays(); 
	} 

	// Function to calculate camera-to-world matrix 
	void calc_matrix() 
	{ 
		Vec<T> forward = (m_Origin - m_LookAt).normalize(); 

		Vec<T> temp = m_Origin; 
		temp.z = T(0); 
		Vec<T> right = forward ^ (temp - m_Origin).normalize(); 

		Vec<T> up = forward ^ right; 

		std::cout << forward << '\n' << up << '\n' << right << '\n'; 

		m_CTWMatrix = Mat<T>{ 
			Vec<T>{    right.x, 	  up.x,  forward.x, T(0) }, 
			Vec<T>{    right.y, 	  up.y,  forward.y, T(0) }, 
			Vec<T>{    right.z, 	  up.z,  forward.z, T(0) }, 
			Vec<T>{ m_Origin.x, m_Origin.y, m_Origin.z, T(1) } 
		}; 
	} 

	// Function to calculate Rays 
	void calc_rays() 
	{ 
		Vec<T> dir_vec{}; 

		m_Rays.resize(m_Width*m_Height); 

		for (size_t i = 0; i < m_Width; ++i) 
			for (size_t j = 0; j < m_Height; ++j) 
			{ 
				T dir_x =  (T(2)*(T(i) + T(0.5))/T(m_Width) - 1)*std::tan(m_FOV/2.)*m_AspectRatio; 
				T dir_y = -(T(2)*(T(j) + T(0.5))/T(m_Height) - 1)*std::tan(m_FOV/2.)*m_AspectRatio; 
				dir_vec = m_CTWMatrix*Vec<T>{ dir_x, dir_y, T(1) }.normalize(); 
				m_Rays[i*m_Height + j] = Ray<T>{ m_Origin, dir_vec }; 
			} 

		m_HasRays = true; 
	} 

	// Function return a single Ray 
	const Ray<T>& get_next_ray() const 
	{ 
		if (m_HasRays) 
		{ 
			m_CurrentRay = (m_CurrentRay + 1) % m_Rays.size(); 

			return m_Rays[m_CurrentRay]; 
		} 

		return Ray<T>{}; 
	} 

	// Function to return a whole array of Rays 
	const std::vector< Ray<T> >& get_rays() const 
	{ 
		return m_Rays;  
	} 

	// Function to return camera-to-world matrix 
	const Mat<T>& get_ctw_matrix() const 
	{ 
		return m_CTWMatrix; 
	}
}; 
