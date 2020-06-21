module; 

export module camera; 

import std.core; 

import vec; 
import ray; 

export template<typename T = float> 
class Camera 
{ 
private: 
	bool has_rays{false}; 
	size_t width{}, height{}; 
	size_t current_ray{}; 
	T fov{}; 
	Vec<T> origin{}, dir{}; 
	std::vector< Ray<T> > rays; 
public: 
	inline 
	Camera() = delete; 

	inline 
	Camera(size_t width_, 
		   size_t height_, 
		   T fov_, 
		   const Vec<T>& origin_, 
		   const Vec<T>& dir_) : 
		width{width_}, 
		height{height_}, 
		fov{fov_}, 
		origin{origin_}, 
		dir{dir_} {} 

	void set_resolution(size_t width_, size_t height_) 
	{ 
		width = width_; 
		height = height_; 

		calc_rays(); 
	} 

	void set_fov(T fov_) 
	{ 
		fov = fov_; 

		calc_rays(); 
	} 

	void set_origin(const Vec<T>& origin_) 
	{ 
		origin = origin_; 

		calc_rays(); 
	} 

	void set_direction(const Vec<T>& dir_) 
	{ 
		dir = dir_; 

		calc_rays(); 
	} 

	void calc_rays() 
	{ 
		rays.resize(width*height); 

		for (size_t i = 0; i < width; ++i) 
			for (size_t j = 0; j < height; ++j) 
			{ 
				T dir_x = (i + 0.5) - width/2; 
				T dir_y = -(j + 0.5) + height/2; 
				T dir_z = -height/(2.0*std::tan(fov/2.)); 
				rays[i + j*height] = { .x = dir_x, .y = dir_y, .z = dir_z }.normalize(); 
			} 

		has_rays = true; 
	} 

	const Vec<T>& get_next_ray() 
	{ 
		current_ray = (current_ray + 1) % (width*height); 

		return rays[current_ray]; 
	}
}; 
