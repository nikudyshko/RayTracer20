module; 

export module application; 

import std.core; 

import vec; 
import polygon; 
import saveppm; 
import render_constants; 

export int main() 
{ 
	std::vector< Vec<float> > pics(WIDTH*HEIGHT); 
	for (int j = 0; j < HEIGHT; ++j) 
		for (int i = 0; i < WIDTH; ++i) 
			pics[j*WIDTH + i] = { j/float(HEIGHT), i/float(WIDTH), 0.0f, 0.0f }; 
	save_ppm(WIDTH, HEIGHT, pics); 

	Vec<float> v0{ 0.5f,  0.0f, 0.0f }; 
	Vec<float> v1{ 0.0f, -0.5f, 0.0f }; 
	Vec<float> v2{-0.5f,  0.0f, 0.0f }; 
	Vec<float> v3{ 0.0f,  0.5f, 0.0f }; 
	Vec<float> v4{ 0.0f,  0.0f, 0.5f }; 

	Polygon<float>{v0, v1, v4}; 
	Polygon<float>{v1, v2, v4}; 
	Polygon<float>{v2, v3, v4}; 
	Polygon<float>{v3, v0, v4}; 
	Polygon<float>{v0, v2, v1}; 
	Polygon<float>{v0, v3, v2}; 
	return 0; 
} 
