module; 

#include <cassert> 

export module renderer; 

import std.core; 

import vec; 
import ray; 
import light; 
import shell; 
import camera; 
import material; 
import hitnode; 
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
	// Optical properties of the environment 
	OpticalBulk<T> m_OptEnv{}; 
	// Lights 
	std::vector< Light<T> > m_Lights{}; 
	// Camera 
	Camera<T> m_Camera{}; 
	// Array of Shells, represents a scene 
	std::vector< Shell<T> > m_Scene{}; 
	// Array of ray path's 
	std::vector< std::unique_ptr<HitNode<T>> > m_Paths{}; 

	std::mutex m_BufferMut, m_OutMut; 

	// Calculates the reflection direction 
	Vec<T> reflect(T incid_norm, const Vec<T>& incid_dir, const Vec<T>& norm) 
	{ 
		return {incid_dir - T(2)*incid_norm*norm}; 
	} 

	// Calculates if the refraction happens. Returns bool flag and refraction direction 
	std::tuple<bool, Vec<T>> refract(T eta_i, T eta_t, T incid_norm, const Vec<T> &incid, const Vec<T> &norm) 
	{ 
		T cos_i = -std::max(T(1), incid_norm); 
		T eta_r = eta_i / eta_t; 
		T cos_sq_t = T(1) - eta_r*eta_r*(T(1) - cos_i*cos_i); 
		return {cos_sq_t > T(0), incid*eta_r + norm*(eta_r*cos_i - std::sqrt(cos_sq_t))}; 
	} 

	// Casts a ray 
	std::unique_ptr<HitNode<T>> cast_ray(size_t depth, T eta_i, Ray<T> &ray) 
	{ 
		if (depth < MAX_DEPTH) 
		{ 
			T eta_t{}; 
			std::unique_ptr<HitNode<T>> res_node{nullptr}; 
			for (Shell<T> &sh : m_Scene) 
			{ 
				if (sh.hit_sphere(ray)) 
				{ 
					if (auto node = sh.path_trace(ray)) 
					{ 
						if (!res_node || (node->dist < res_node->dist))  
						{ 
							res_node = std::move(node); 
							eta_t = sh.get_opt_prop().refraction; 
						} 
					} 
				} 
			} 

			if (res_node) 
			{ 
				res_node->depth = depth; 

				res_node->ray_orig = ray.origin; 
				res_node->ray_dir = ray.dir; 

				const Vec<T> &incid = ray.dir; 
				const Vec<T> &norm = res_node->normal; 

				T incid_norm = incid*norm; 

				ray.origin = (incid_norm < T(0)) ? (res_node->gx_point + T(1e-3)*norm) : (res_node->gx_point - T(1e-3)*norm); 

				ray.dir = reflect(incid_norm, incid, norm).normalize(); 
				res_node->refl_comp = cast_ray(depth + 1, eta_t, ray); 

				// if (auto[refr, dir] = refract(eta_i, eta_t, incid_norm, incid, norm); refr) 
				// { 
				// 	ray.origin = (incid_norm < T(0)) ? (res_node->gx_point - T(1e-3)*norm) : (res_node->gx_point + T(1e-3)*norm); 
				// 	ray.dir = dir; 
				// 	res_node->refr_comp = cast_ray(depth + 1, eta_t, ray); 
				// } 
			} 
			return res_node;  
		} 
		return nullptr; 
	} 

	// Function, that performs reccurent light-shadow tracing and color blending 
	Vec<T> light_shadow_trace(const std::unique_ptr<HitNode<T>> &node, Ray<T> &r) 
	{ 
		if (!node) 
			return {T(0.2), T(0.3), T(0.7)}; 

		Vec<T> refl_color{light_shadow_trace(node->refl_comp, r)}; 
		Vec<T> refr_color{light_shadow_trace(node->refr_comp, r)}; 

		size_t shell_id = node->shell_id; 
		size_t surface_id = node->surface_id; 
		const Vec<T> &ray_dir = node->ray_dir; 
		const Vec<T> &gx_point = node->gx_point; 
		const Vec<T> &norm = node->normal; 
		const OpticalSurface<T> &s = node->mat; 

		T diffuse_light{T(0)}, specular_light{T(0)}; 

		for (const Light<T> &l : m_Lights) 
		{ 
			bool shadowed{false}; 

			T light_distance{(l.position - gx_point).sq_norm()}; 
			Vec<T> light_dir{(l.position - gx_point).normalize()}; 

			T light_norm{light_dir*norm}; 

			Vec<T> shadow_origin{(light_norm < T(0)) ? gx_point - T(1e-3)*norm : gx_point + T(1e-3)*norm}; 
			Ray<T> shadow_ray{shadow_origin, light_dir}; 

			for (const Shell<T> &sh : m_Scene) 
			{ 
				if (shadowed = sh.shadow_trace(shell_id, surface_id, light_distance, shadow_ray, l)) 
					break; 
			} 

			if (shadowed) 
				continue; 

			Vec<T> refl = {reflect(light_norm, light_dir, norm)}; 
			diffuse_light += l.intensity*std::max(T(0), light_norm); 
			specular_light += l.intensity*std::pow(std::max(T(0), refl*ray_dir), s.specular); 
		} 

		return diffuse_light*s.reflection[0]*s.color + Vec<T>{specular_light*s.reflection[1]} + refl_color*s.reflection[2] + refr_color*s.reflection[3]; 
	} 	

	// Function, that starts recurrent path-tracing 
	void path_trace(std::vector< Ray<T> > &rays, std::vector< Vec<T> > &frame) 
	{ 
		for (Ray<T> &r : rays) 
		{ 
			std::unique_ptr<HitNode<T>> path{cast_ray(0, m_OptEnv.refraction, r)}; 
			Vec<T> color{light_shadow_trace(path, r)}; 
			
			m_BufferMut.lock(); 
			m_Paths[r.pc.y*m_Width + r.pc.x] = std::move(path); 
			frame[r.pc.y*m_Width + r.pc.x] = color; 
			m_BufferMut.unlock(); 
		} 
	} 

public: 
	// Default constructor 
	Renderer() {} 

	// Sets optical properties of environment 
	void set_opt_prop(const OpticalBulk<T> &opt_b) 
	{ m_OptEnv = opt_b; } 

	// Adds a light-source 
	void add_light(const Light<T>& light) 
	{ 
		m_Lights.push_back(light); 
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

		m_Paths.resize(m_Width*m_Height); 

		std::vector< std::thread > threads(core_count); 

		for (size_t i = 0; i < core_count; ++i) 
			path_trace(thread_rays[i], framebuffer); 

		// T sc = 1/max_dist; 
		// for (auto &v : framebuffer) 
		// 	v *= sc; 

		// 	threads[i] = std::thread(&Renderer::path_trace, this, std::ref(thread_rays[i]), std::ref(framebuffer)); 

		// for (std::thread &t : threads) 
		// 	t.join(); 

		save_ppm(m_Width, m_Height, framebuffer); 
	} 
}; 
