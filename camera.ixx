module; 

export module camera; 

import std.core; 

import vec; 
import ray; 

export template<typename T = float> 
class Camera 
{ 
private: 
	bool has_rays; 
	size_t width, height; 
	Vec<T> pos, dir; 
	std::vector< Ray<T> > rays; 
public: 
	inline 
	Camera() = delete; 

	inline 
	Camera(size_t width_, 
		   size_t height_, 
		   const Vec<T>& pos_, 
		   const Vec<T>& dir_) : 
		width{width_}, 
		height{height_}, 
		pos{pos_}, 
		dir{dir_} {} 

	inline 
	void set_resolution(size_t width_, size_t height_) 
	{ 
		width = width_; 
		height = height_; 
	}

	inline 
	void set_position(const Vec<T>& pos_)  { pos = pos_; } 

	inline 
	void set_direction(const Vec<T>& dir_) { dir = dir_; } 
}; 
