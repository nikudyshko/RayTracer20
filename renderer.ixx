module; 

export module renderer; 

import std.core; 

import vec; 
import ray; 
import shell; 

// Class to implement a rendering cycle 
export template<typename T> 
class Renderer 
{ 
private: 
    // Flag to show if the Renderer has a Camera 
    bool m_HasCamera{false}; 
    // Flag to show if the Renderer has a scene 
    bool m_HasScene{false}; 
    // Camera 
    Camera<T> m_Camera{}; 
    // Shell, that contains other shells and represents a scene 
    Shell<T> m_Scene{}; 

    // Function, that performs recurrent ray-tracing 
    void ray_trace() {}; 
public: 
    // Default constructor 
    Renderer() {} 
    // Constructs a Renderer from a Shell 
    Renderer(Shell<T> sh) : 
        scene{sh} {} 

    // Sets the Camera 
    void set_camera(const Camera<T>& camera) 
    { 
        m_Camera = camera; 
        m_HasCamera = true; 
    } 

    // Sets the scene 
    void set_scene(const Shell<T>& scene) 
    { 
        m_Scene = scene; 
        m_HasScene = true; 
    }

    // Function, that performs a rendering 
    void render() 
    { 
        const std::vector< Ray<T> >& rays = m_Camera.get_rays(); 
        std::vector< Vec<T> > framebuffer(rays.size()); 

        size_t core_count = std::thread::hardware_concurrency(); 

        std::vector< std::vector< Ray<T> > > thread_rays(core_count); 
        for (size_t i = 0; i < core_count; ++i) 
            for (size_t j = 0; j < rays.size()/core_count; ++j) 
                thread_rays[i].push_back(rays[i*rays.size()/core_count + j]); 

        std::vector< std::thread > threads(core_count); 

        for (size_t i = 0; i < core_count; ++i) 
            threads[i] = std::thread(&Renderer::ray_trace, this, i, thread_rays[i], framebuffer); 
        
        for (std::thread& t : threads) 
            threads[i].join(); 

        save_ppm(WIDTH, HEIGHT, framebuffer); 
    } 
}; 
