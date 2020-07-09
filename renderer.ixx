module; 

export module renderer; 

import std.core; 

import vec; 
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

    // Function, that performs a rendering 
    void render() {} 
}; 
