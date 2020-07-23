module; 

export module render_constants; 

import std.core; 

// Defines parameters for rendering process 
export 
{ 
             constexpr bool   BF_CULLING{false};        // Enable/Disable back-face culling 
    volatile constexpr size_t WIDTH{1024};              // Image width. Volatile added to avoid compiler optimization 
    volatile constexpr size_t HEIGHT{768};              // Image height. Volatile added to avoid compiler optimization 
             constexpr double EPS{0.0001};              // EPS for floating-point operations 
} 
