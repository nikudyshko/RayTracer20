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
			auto& hit_spots = r.hit_spots; 
			for (auto hs_it = hit_spots.rbegin(); hs_it != hit_spots.rend(); ++hs_it) 
			{ 
				size_t shell_id = hs_it->second.shell_id; 
				size_t surface_id = hs_it->second.surface_id; 
				Vec<T>& gx_point = hs_it->second.gx_point; 
				Vec<T>& norm = hs_it->second.normal; 
				OpticalSurface<T>& s = hs_it->second.mat; 

				T diffuse_light{T(0)}, specular_light{T(0)}; 

				for (const Light<T>& l : lights) 
				{ 
					bool shadowed{false}; 
					T light_distance{(l.position - gx_point).sq_norm()}; 
					Vec<T> light_dir{(l.position - gx_point).normalize()}; 
					Ray<T> shadow_ray{ gx_point, light_dir }; 

					for (const Shell<T>& sh : m_Scene) 
					{ 
						if (shadowed = sh.shadow_trace(shell_id, surface_id, light_distance, shadow_ray, l)) 
							break; 
					} 

					if (shadowed) 
						continue; 

					T light_norm = light_dir*norm; 
					Vec<T> reflect{light_dir - T(2)*light_norm*norm}; 
					diffuse_light += l.intensity*std::max(T(0), light_norm); 
					specular_light += l.intensity*std::pow(std::max(T(0), -reflect*r.dir), s.specular); 
				} 
				r.lighting[hs_it->first].diffuse_lights.push_back(diffuse_light); 
				r.lighting[hs_it->first].specular_lights.push_back(specular_light); 
			}
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
				color = 0; 
				for (auto l_it = r.lighting.rbegin(); l_it != r.lighting.rend(); ++l_it) 
				{ 
					OpticalSurface<T>& s = r.hit_spots[l_it->first].mat; 
					Lighting<T>& l = l_it->second; 
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
