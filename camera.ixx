module; 

import std.core; 

export module camera; 

export template<typename T = float> 
class Camera 
{ 
private: 
	size_t width, height; 
	Vec<T> dir; 
public: 
	Camera() = delete; 
	Camera(size_t width_, size_t height_, const Vec<T>& dir_) : 
		width{width_}, height{height_}, dir{dir_} {}; 
}; 
