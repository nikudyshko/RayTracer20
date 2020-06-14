module; 

export module camera; 

import vec; 

export template<typename T = float> 
class Camera 
{ 
private: 
	size_t width, height; 
	Vec<T> pos, dir; 
public: 
	Camera() = delete; 
	Camera(size_t width_, 
		   size_t height_, 
		   const Vec<T>& pos_, 
		   const Vec<T>& dir_) : 
		width{width_}, 
		height{height_}, 
		pos{pos_}, 
		dir{dir_} {} 

	inline 
	void set_position(const Vec<T>& pos_)  { pos = pos_; } 

	inline 
	void set_direction(const Vec<T>& dir_) { dir = dir_; } 
}; 
