module; 

export module hitnode; 

import std.core; 

import vec; 
import material; 

// Struct that describes a rays' hit spot 
export template<typename T> 
struct HitNode 
{ 
    size_t depth{}, shell_id{}, surface_id{}; 
    T dist{std::numeric_limits<T>::max()}; 
    Vec<T> ray_orig{}, ray_dir{}; 
    Vec<T> normal{}, lx_point{}, gx_point{}; 
    OpticalSurface<T> mat{}; 
    std::unique_ptr<HitNode<T>> refl_comp{nullptr}, refr_comp{nullptr}; 
}; 
