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
	Vec<float> v1{1, 2, 4}; 
	Vec<float> v2{4, 5, 3}; 
	Vec<float> v3{7, 8, 9}; 
	Polygon<float> p{v1, v2, v3}; 
	Vec<float> norm = p.get_normal(); 
	std::cout << norm << '\n'; 
	return 0; 
} 
