module; 

export module render_constants; 

import std.core; 

// Defines parameters for rendering process 
export 
{ 
             constexpr bool   BF_CULLING{false};         // Enable/Disable back-face culling 
    volatile constexpr size_t WIDTH{1024};              // Image width. Volatile added to avoid compiler optimization 
    volatile constexpr size_t HEIGHT{768};              // Image height. Volatile added to avoid compiler optimization 
             constexpr size_t MAX_DEPTH{4};             // Max depth of recurrent tracing 
             constexpr double EPS{0.0001};              // EPS for floating-point operations 
             constexpr double MAX_DIST{std::numeric_limits<float>::max()/2.0}; // Max distance the ray can propagate 
} 
