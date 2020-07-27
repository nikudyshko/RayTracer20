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

	Vec<float> m_v0{  0.5f,  0.0f, 0.0f }; 
	Vec<float> m_v1{  0.0f, -0.5f, 0.0f }; 
	Vec<float> m_v2{ -0.5f,  0.0f, 0.0f }; 
	Vec<float> m_v3{  0.0f,  0.5f, 0.0f }; 
	Vec<float> m_v4{  0.0f,  0.0f, 0.5f }; 
	Vec<float> p_v0{  0.0f,  0.0f, 0.0f }; 
	Vec<float> p_v1{ -2.0f,  0.0f, 0.0f }; 
	Vec<float> p_v2{  2.0f,  0.0f, 0.0f }; 
	Vec<float> p_v3{  0.0f, -2.0f, 0.0f }; 
	Vec<float> p_v4{  0.0f,  2.0f, 0.0f }; 
	Vec<float> p_v5{ -2.0f, -2.0f, 0.0f }; 
	Vec<float> p_v6{ -2.0f,  2.0f, 0.0f }; 
	Vec<float> p_v7{  2.0f, -2.0f, 0.0f }; 
	Vec<float> p_v8{  2.0f,  2.0f, 0.0f }; 

	Vec<float> refl0{ 0.6f, 0.3f, 0.1f, 0.0f}; 
	Vec<float> refl1{ 0.0f, 0.5f, 0.1f, 0.8f}; 
	Vec<float> refl2{ 0.9f, 0.1f, 0.0f, 0.0f}; 
	Vec<float> refl3{0.0f, 10.0f, 0.8f, 0.0f}; 
	Vec<float> color0{1.0f, 0.0f, 0.0f}; 
	Vec<float> color1{0.0f, 1.0f, 0.0f}; 
	Vec<float> color2{0.0f, 0.0f, 1.0f}; 
	Vec<float> color3{1.0f, 0.0f, 1.0f}; 
	Vec<float> color4{0.0f, 1.0f, 1.0f}; 
	Vec<float> color5{1.0f, 1.0f, 0.0f}; 

	OpticalSurface<float> opt_s0{50.0f, refl0, color0}; 
	OpticalSurface<float> opt_s1{125.0f, refl2, color1}; 
	OpticalSurface<float> opt_s2{10.0f, refl0, color2}; 
	OpticalSurface<float> opt_s3{1425.0f, refl2, color3}; 
	OpticalSurface<float> opt_s4{0.0f, refl0, color4}; 
	OpticalSurface<float> opt_s5{0.0f, refl2, color5}; 
	OpticalBulk<float> opt_b{1.0f, 1.0f};  

	Polygon<float> m_p0{m_v0, m_v4, m_v1}; 
	Polygon<float> m_p1{m_v1, m_v4, m_v2}; 
	Polygon<float> m_p2{m_v2, m_v4, m_v3}; 
	Polygon<float> m_p3{m_v3, m_v4, m_v0}; 
	Polygon<float> m_p4{m_v0, m_v1, m_v2}; 
	Polygon<float> m_p5{m_v0, m_v2, m_v3}; 
	Polygon<float> p_p0{p_v0, p_v1, p_v3}; 
	Polygon<float> p_p1{p_v0, p_v4, p_v1}; 
	Polygon<float> p_p2{p_v2, p_v4, p_v0}; 
	Polygon<float> p_p3{p_v0, p_v3, p_v2}; 
	Polygon<float> p_p4{p_v1, p_v5, p_v3}; 
	Polygon<float> p_p5{p_v1, p_v4, p_v6}; 
	Polygon<float> p_p6{p_v2, p_v8, p_v4}; 
	Polygon<float> p_p7{p_v2, p_v3, p_v7}; 
	m_p0.calc_normal(); 
	m_p1.calc_normal(); 
	m_p2.calc_normal(); 
	m_p3.calc_normal(); 
	m_p4.calc_normal(); 
	m_p5.calc_normal(); 
	p_p0.calc_normal(); 
	p_p1.calc_normal(); 
	p_p2.calc_normal(); 
	p_p3.calc_normal(); 
	p_p4.calc_normal(); 
	p_p5.calc_normal(); 
	p_p6.calc_normal(); 
	p_p7.calc_normal(); 

	Surface<float> m_s0{m_p0, opt_s0}; 
	Surface<float> m_s1{m_p1, opt_s1}; 
	Surface<float> m_s2{m_p2, opt_s2}; 
	Surface<float> m_s3{m_p3, opt_s3}; 
	Surface<float> m_s4{m_p4, opt_s4}; 
	Surface<float> m_s5{m_p5, opt_s5}; 
	Surface<float> p_s0{p_p0, opt_s3}; 
	Surface<float> p_s1{p_p1, opt_s2}; 
	Surface<float> p_s2{p_p2, opt_s1}; 
	Surface<float> p_s3{p_p3, opt_s0}; 
	Surface<float> p_s4{p_p4, opt_s3}; 
	Surface<float> p_s5{p_p5, opt_s2}; 
	Surface<float> p_s6{p_p6, opt_s1}; 
	Surface<float> p_s7{p_p7, opt_s0}; 

	std::vector< Surface<float> > plane_mesh{p_s0, p_s1, p_s2, p_s3, p_s4, p_s5, p_s6, p_s7}; 
	std::vector< Surface<float> > pyramid_mesh{m_s0, m_s1, m_s2, m_s3, m_s4, m_s5}; 

	Shell<float> plane{}; 
	plane.set_opt_prop(opt_b); 
	plane.add_surfaces(plane_mesh); 
	plane.calc_bound_sphere(); 

	Shell<float> pyramid{}; 
	pyramid.set_opt_prop(opt_b); 
	pyramid.add_surfaces(pyramid_mesh); 
	pyramid.calc_bound_sphere(); 

	std::vector< Shell<float> > scene{}; 
	scene.push_back(plane); 
	scene.push_back(pyramid); 

	Vec<float> origin{3.0f, 3.0f, 3.0f}; 
	Vec<float> look_at{0.0f, 0.0f, 0.0f}; 

	Camera<float> cam{WIDTH, HEIGHT, 3.14f/3.0f, origin, look_at}; 
	cam.calc_matrix({0.0f, 0.0f, -1.0f}); 
	cam.calc_rays(); 

	Renderer<float> rend{}; 

	Light<float> l1{1.0f, Vec<float>{3.0f, 1.0f, 1.0f}, Vec<float>{3.0f, 3.0f, 3.0f}}; 
	Light<float> l2{0.5f, Vec<float>{0.3f, 0.5f, 0.4f}, Vec<float>{-3.0f, -3.0f, 3.0f}}; 
	Light<float> l3{0.3f, Vec<float>{0.1f, 0.1f, 0.2f}, Vec<float>{-7.0f, 3.0f, 4.0f}}; 
	rend.add_light(l1);  
	rend.add_light(l2); 
	rend.add_light(l3); 
	rend.set_camera(cam); 
	rend.set_scene(scene); 
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Start = std::chrono::high_resolution_clock::now(); 
	rend.render(); 
	std::chrono::time_point<std::chrono::high_resolution_clock> m_Stop = std::chrono::high_resolution_clock::now(); 
	int ms = std::chrono::duration_cast<std::chrono::milliseconds>(m_Stop - m_Start).count(); 
	std::cout << "Render time: " << ms << " ms\n"; 

	return 0; 
} 
