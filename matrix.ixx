module; 

export module mat;  

import std.core; 

import vec; 

// Struct to implement 4*4 matrix 
export template<typename T = float> 
struct Mat 
{ 
    // Matrix is represented as vector of column vectors (for now) 
    std::vector< Vec<T> > cols{}; 

    // Default constructor 
    inline 
    Mat () {} 

    // Constructs Matrix out of four vectors 
    inline 
    Mat (std::initializer_list< Vec<T> > l) : 
        cols{l} {} 
}; 

// Matrix-Vector multiplication 
export template<typename T, typename U> 
auto operator* (const Mat<T>& m, const Vec<U>& v) -> Vec<decltype(m.cols[0].x*v.x)> 
{ 
    using W = decltype(m.cols[0].x*v.x); 

    Vec<W> res{}; 

    for (size_t i = 0; i < m.cols.size(); ++i) 
        res[i] = m.cols[i]*v; 

    return res; 
}
