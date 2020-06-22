module; 

export module render_constants; 

import std.core; 

export 
{ 
             constexpr bool   BF_CULLING{false}; 
    volatile constexpr size_t WIDTH{1024}; 
    volatile constexpr size_t HEIGHT{768}; 
             constexpr double EPS{0.0001};  
} 
