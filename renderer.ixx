module; 

#include <cassert> 

export module renderer; 

import std.core; 

import vec; 
import ray; 
import shell; 

template<typename T> 
struct RenderNode 
{ 
    std::shared_ptr< RenderNode<T> > parent_node{nullptr}; 
    std::vector< std::shared_ptr< RenderNode<T> > child_nodes{}; 
    SHell<T> sh{}; 
}; 

// Class to implement a rendering cycle 
export template<typename T> 
class Renderer 
{ 
private: 
    // Flag to show if the Renderer has a Camera 
    bool m_HasCamera{false}; 
    // Flag to show if the Renderer has a scene 
    bool m_HasScene{false}; 
    // Flag to show if the Renderer has a Rendering Tree 
    bool m_HasRenderingTree{false}; 
    // Camera 
    Camera<T> m_Camera{}; 
    // Shell, that contains other shells and represents a scene 
    Shell<T> m_Scene{}; 
    // Rendering tree 
    RenderNode<T>* m_RenderingTree{nullptr}; 

    // Function, that performs a recurrent build of rendering tree 
    RenderNode<T>* add_render_nodes(const RenderNode<T>* parent_node, const Shell<T>& sh) 
    { 
        RenderNode<T>* node = new RenderNode<T>(); 
        
        node->parent_node = parent_node; 
        node->sh = sh; 

        const std::vector< Shell<T> >& inner_shells = sh.get_inner_shells(); 
        for (const Shell<T>& shell : inners_shells) 
        { 
            RenerNode<T>* child_node = add_render_nodes(node, shell); 
            node->child_nodes.push_back(child_node); 
        } 

        return node; 
    } 

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
        assert(m_HasCamera && m_HasScene && m_HasRenderTree); 
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
