module; 

export module application; 

import std.core; 

import vec; 
import shell; 
import polygon; 
import surface; 
import saveppm; 
import material; 
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

	Vec<float> refl{0.1f, 0.1f, 0.1f}; 
	Vec<float> color{0.5f, 0.7f, 1.0f}; 

	OpticalSurface<float> opt_s{0.0f, refl, color}; 
	OpticalBulk<float> opt_b{1.0f, 1.0f}; 

	Polygon<float> p0{v0, v1, v4}; 
	Polygon<float> p1{v1, v2, v4}; 
	Polygon<float> p2{v2, v3, v4}; 
	Polygon<float> p3{v3, v0, v4}; 
	Polygon<float> p4{v0, v2, v1}; 
	Polygon<float> p5{v0, v3, v2}; 

	Surface<float> s0{p0, opt_s}; 
	Surface<float> s1{p1, opt_s}; 
	Surface<float> s2{p2, opt_s}; 
	Surface<float> s3{p3, opt_s}; 
	Surface<float> s4{p4, opt_s}; 
	Surface<float> s5{p5, opt_s}; 

	std::vector< Surface<float> > mesh{s0, s1, s2, s3, s4, s5}; 

	Shell<float> sh{}; 
	sh.set_opt_prop(opt_b); 
	sh.add_polygons(mesh); 
	return 0; 
} 
