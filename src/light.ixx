module; 

export module light; 

import std.core; 

import vec; 

// Structure to implement a point-like lightsource 
export template<typename T> 
struct Light 
{ 
    // Intensity of a lightsource
    T intensity{}; 
    // Color of a lightsource 
    Vec<T> color{}; 
    // Position of a lightsource 
    Vec<T> position{}; 

    // Default constructor 
    Light() {} 

    // Constructs a lightsource from intensity, color and position 
    Light(T intensity_, 
          const Vec<T>& color_, 
          const Vec<T>& position_) : 
        intensity{intensity_}, 
        color{color_}, 
        position{position_} {} 
}; 
