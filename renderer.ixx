module; 

#include <cassert> 

export module renderer; 

import std.core; 

import vec; 
import ray; 
import light; 
import shell; 
import camera; 

template<typename T> 
struct RenderNode 
{ 
	RenderNode<T>* parent_node{nullptr}; 
	std::vector< RenderNode<T>* > child_nodes{}; 
	Shell<T> sh{}; 
}; 

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
	// Flag to show if the Renderer has a Rendering Tree 
	bool m_HasRenderingTree{false}; 
	// Camera resolution 
	size_t m_Width{}, m_Height{}; 
	// Lights 
	std::vector< Light<T> > lights{}; 
	// Camera 
	Camera<T> m_Camera{}; 
	// Shell, that contains other shells and represents a scene 
	Shell<T> m_Scene{}; 
	// Rendering tree 
	RenderNode<T>* m_RenderingTree{nullptr}; 

	std::mutex m_BufferMut, m_OutMut; 

	// Function, that performs a recurrent build of rendering tree 
	RenderNode<T>* add_render_nodes(RenderNode<T>* parent_node, const Shell<T>& sh) 
	{ 
		RenderNode<T>* node = new RenderNode<T>(); 

		node->parent_node = parent_node; 
		node->sh = sh; 

		const std::vector< Shell<T> >& inner_shells = sh.get_inner_shells(); 
		for (const Shell<T>& shell : inner_shells) 
		{ 
			RenderNode<T>* child_node = add_render_nodes(node, shell); 
			node->child_nodes.push_back(child_node); 
		} 

		return node; 
	} 

	// Function, that performs recurrent ray-tracing 
	void ray_trace(std::vector< Ray<T> >& rays, std::vector< Vec<T> >& frame, RenderNode<T>* render_node) 
	{ 
		size_t hited = 0; 
		size_t traced = 0; 
		for (Ray<T>& r : rays) 
		{ 
			if (render_node->sh.hit_sphere(r))  
			{ 
				++hited; 
				if (render_node->sh.trace(r)) 
				{ 
					++traced; 

					T diffuse_light{T(0)}; 

					auto hit_spot = r.hit_spots.begin()->second; 
					Vec<T> light_dir{};  

					for (Light<T>& l : lights) 
					{ 
						light_dir = (l.position - std::get<1>(hit_spot)).normalize();
						diffuse_light += l.intensity*std::max(T(0), light_dir*std::get<2>(hit_spot)); 
					} 
					Vec<T> s_color = diffuse_light*std::get<3>(hit_spot); 
					m_BufferMut.lock();  
					frame[r.pc.y*m_Width + r.pc.x] = s_color;  
					m_BufferMut.unlock(); 
				}
			}  
		} 
		m_OutMut.lock(); 
		std::cout << "Hited: " << hited << '\n'; 
		std::cout << "Traced: " << traced << '\n'; 
		m_OutMut.unlock(); 
	} 

public: 
	// Default constructor 
	Renderer() {} 

	// Adds a light-source 
	void add_light(const Light<T>& light) 
	{ 
		lights.push_back(light); 
		m_HasLights = true; 
		// for (Light<T>& l : lights) 
		// 	std::cout << l.position << '\n'; 
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
	void set_scene(const Shell<T>& scene) 
	{ 
		m_Scene = scene; 
		m_HasScene = true; 
	} 

	// Builds Rendering Tree 
	void build_rendering_tree() 
	{ 
		assert(m_HasScene); 
		m_RenderingTree = add_render_nodes(nullptr, m_Scene); 
		m_HasRenderingTree = true; 
	}

	// Function, that performs a rendering 
	void render() 
	{ 
		assert(m_HasLights && m_HasCamera && m_HasScene && m_HasRenderingTree); 
		const std::vector< Ray<T> >& rays = m_Camera.get_rays(); 
		std::vector< Vec<T> > framebuffer(rays.size()); 

		size_t core_count = std::thread::hardware_concurrency(); 

		std::vector< std::vector< Ray<T> > > thread_rays(core_count); 
		for (size_t i = 0; i < core_count; ++i) 
			for (size_t j = 0; j < rays.size()/core_count; ++j) 
				thread_rays[i].push_back(rays[i*rays.size()/core_count + j]); 

		std::vector< std::thread > threads(core_count); 

		for (size_t i = 0; i < core_count; ++i) 
			threads[i] = std::thread(&Renderer::ray_trace, this, std::ref(thread_rays[i]), std::ref(framebuffer), m_RenderingTree); 
		
		for (std::thread& t : threads) 
		    t.join(); 

		save_ppm(m_Width, m_Height, framebuffer); 
	} 
}; 
