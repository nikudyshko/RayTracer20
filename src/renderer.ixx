module; 

#include <cassert> 

export module renderer; 

import std.core; 

import vec; 
import ray; 
import light; 
import shell; 
import camera; 
import render_constants; 

// Class to implement a rendering cycle 
export template<typename T> 
class Renderer 
{ 
private: 
	// Flag to show if the Renderer has Lights 
	bool m_HasLights{false}; 
	// Flag to show if the Renderer has a Camera 
	bool m_HasCamera{false}; 
	// Flag to show if the Renderer has a scene 
	bool m_HasScene{false}; 
	// Camera resolution 
	size_t m_Width{}, m_Height{}; 
	// Lights 
	std::vector< Light<T> > lights{}; 
	// Camera 
	Camera<T> m_Camera{}; 
	// Array of Shells, represents a scene 
	std::vector< Shell<T> > m_Scene{}; 

	std::mutex m_BufferMut, m_OutMut; 

	// Casts a ray 
	void cast_ray(size_t depth, Ray<T>& ray) 
	{ 
		if (depth < MAX_DEPTH) 
		{ 
			for (Shell<T>& sh : m_Scene) 
			{ 
				if (sh.hit_sphere(ray)) 
					sh.path_trace(depth, ray); 
			}
		}
	}

	// Function, that performs recurrent path-tracing 
	void path_trace(std::vector< Ray<T> >& rays) 
	{ 
		for (Ray<T>& r : rays) 
		{ 
			cast_ray(0, r); 
		} 
	} 

	// Function, that performs reccurent light-shadow tracing 
	void light_shadow_trace(std::vector< Ray<T> >& rays) 
	{ 
		for (Ray<T>& r : rays) 
		{ 
			for (Shell<T>& sh : m_Scene) 
				sh.light_shadow_trace(0, r, lights); 
		}
	}

	// Blends tracing results into a picture buffer 
	void blend(std::vector< Ray<T> >& rays, std::vector< Vec<T> >& frame) 
	{ 
		Vec<T> color{}; 
		for (Ray<T>& r : rays) 
		{ 
			if (r.hit) 
			{ 
				m_OutMut.lock(); 
				color = 0; 
				for (auto l_it = r.lighting.rbegin(); l_it != r.lighting.rend(); ++l_it) 
				{ 
					OpticalSurface<T> s = r.hit_spots[l_it->first].mat; 
					Lighting<T> l = l_it->second; 
					for (size_t i = 0; i < l.diffuse_lights.size(); ++i) 
						color += l.diffuse_lights[i]*s.reflection[0]*s.color + Vec<T>{l.specular_lights[i]*s.reflection[1]}; 
				} 
			} 
			else 
				color = {T(0.2), T(0.3), T(0.7)}; 
			m_BufferMut.lock(); 
			frame[r.pc.y*m_Width + r.pc.x] = color;  
			m_BufferMut.unlock(); 
		} 
	}

public: 
	// Default constructor 
	Renderer() {} 

	// Adds a light-source 
	void add_light(const Light<T>& light) 
	{ 
		lights.push_back(light); 
		m_HasLights = true; 
	} 

	// Sets the Camera 
	void set_camera(const Camera<T>& camera) 
	{ 
		m_Camera = camera; 
		auto[w, h] = m_Camera.get_resolution(); 
		m_Width = w; 
		m_Height = h; 
		m_HasCamera = true; 
	} 

	// Sets the scene 
	template<typename Container> 
	void set_scene(const Container& scene) 
	{ 
		m_Scene.clear(); 
		std::copy(scene.begin(), scene.end(), std::back_inserter(m_Scene)); 
		for (size_t i = 0; i < m_Scene.size(); ++i) 
			m_Scene[i].set_shell_id(i); 
		m_HasScene = m_Scene.size() > 0; 
	} 

	// Function, that performs a rendering 
	void render() 
	{ 
		assert(m_HasLights && m_HasCamera && m_HasScene); 
		const std::vector< Ray<T> >& rays = m_Camera.get_rays(); 
		std::vector< Vec<T> > framebuffer(rays.size()); 

		size_t core_count = std::thread::hardware_concurrency(); 

		std::vector< std::vector< Ray<T> > > thread_rays(core_count); 
		for (size_t i = 0; i < core_count; ++i) 
			for (size_t j = 0; j < rays.size()/core_count; ++j) 
				thread_rays[i].push_back(rays[i*rays.size()/core_count + j]); 

		std::vector< std::thread > threads(core_count); 

		for (size_t i = 0; i < core_count; ++i) 
			threads[i] = std::thread(&Renderer::path_trace, this, std::ref(thread_rays[i])); 
		
		for (std::thread& t : threads) 
		    t.join(); 

		for (size_t i = 0; i < core_count; ++i) 
			threads[i] = std::thread(&Renderer::light_shadow_trace, this, std::ref(thread_rays[i])); 

		for (std::thread& t : threads) 
			t.join(); 

		for (size_t i = 0; i < core_count; ++i) 
			threads[i] = std::thread(&Renderer::blend, this, std::ref(thread_rays[i]), std::ref(framebuffer)); 

		for (std::thread& t : threads) 
			t.join(); 

		save_ppm(m_Width, m_Height, framebuffer); 
	} 
}; 
