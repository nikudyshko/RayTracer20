module; 

export module application; 

import std.core; 

import vec; 
import ray; 
import mat; 
import shell; 
import light; 
import camera;  
import polygon; 
import surface; 
import saveppm; 
import material; 
import renderer; 
import render_constants; 

import test; 

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
	Vec<float> color0{1.0f, 0.0f, 0.0f}; 
	Vec<float> color1{0.0f, 1.0f, 0.0f}; 
	Vec<float> color2{0.0f, 0.0f, 1.0f}; 
	Vec<float> color3{1.0f, 0.0f, 1.0f}; 
	Vec<float> color4{0.0f, 1.0f, 1.0f}; 
	Vec<float> color5{1.0f, 1.0f, 0.0f}; 

	OpticalSurface<float> opt_s0{0.0f, refl, color0}; 
	OpticalSurface<float> opt_s1{0.0f, refl, color1}; 
	OpticalSurface<float> opt_s2{0.0f, refl, color2}; 
	OpticalSurface<float> opt_s3{0.0f, refl, color3}; 
	OpticalSurface<float> opt_s4{0.0f, refl, color4}; 
	OpticalSurface<float> opt_s5{0.0f, refl, color5}; 
	OpticalBulk<float> opt_b{1.0f, 1.0f};  

	Polygon<float> p0{v0, v4, v1}; 
	Polygon<float> p1{v1, v4, v2}; 
	Polygon<float> p2{v2, v4, v3}; 
	Polygon<float> p3{v3, v4, v0}; 
	Polygon<float> p4{v0, v1, v2}; 
	Polygon<float> p5{v0, v2, v3}; 
	p0.calc_normal(); 
	p1.calc_normal(); 
	p2.calc_normal(); 
	p3.calc_normal(); 
	p4.calc_normal(); 
	p5.calc_normal(); 

	Surface<float> s0{p0, opt_s0}; 
	Surface<float> s1{p1, opt_s1}; 
	Surface<float> s2{p2, opt_s2}; 
	Surface<float> s3{p3, opt_s3}; 
	Surface<float> s4{p4, opt_s4}; 
	Surface<float> s5{p5, opt_s5}; 

	std::vector< Surface<float> > mesh{s0, s1, s2, s3, s4, s5}; 

	Shell<float> sh{}; 
	sh.set_opt_prop(opt_b); 
	sh.add_surfaces(mesh); 
	sh.calc_bound_sphere(); 

	Vec<float> origin{2.0f, 2.0f, 2.0f}; 
	Vec<float> look_at{0.0f, 0.0f, 0.0f}; 

	Camera<float> cam{WIDTH, HEIGHT, 3.14f/3.0f, origin, look_at}; 
	cam.calc_matrix(); 
	cam.calc_rays(); 

	Renderer<float> rend{}; 

	Light<float> l1{1.0f, Vec<float>{3.0f, 1.0f, 1.0f}, Vec<float>{3.0f, 3.0f, 3.0f}}; 
	Light<float> l2{0.5f, Vec<float>{0.3f, 0.5f, 0.4f}, Vec<float>{-3.0f, -3.0f, 3.0f}}; 
	Light<float> l3{0.3f, Vec<float>{0.1f, 0.1f, 0.2f}, Vec<float>{-7.0f, 3.0f, 4.0f}}; 
	rend.add_light(l1);  
	rend.add_light(l2); 
	rend.add_light(l3); 
	rend.set_camera(cam); 
	rend.set_scene(sh); 
	rend.build_rendering_tree(); 
	rend.render(); 

	return 0; 
} 
