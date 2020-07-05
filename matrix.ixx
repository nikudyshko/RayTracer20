module; 

export module mat;  

import std.core; 

import vec; 

// Struct to implement 4*4 matrix 
export template<typename T> 
struct Mat 
{ 
    // Matrix is represented as vector of column vectors (for now) 
    std::vector< Vec<T> > cols{}; 

    // Default constructor 
    inline 
    Mat () {} 

    // Copy constructor 
    inline 
    Mat (const Mat<T>& m) 
    { 
        for (auto& v : m.cols) 
            cols.push_back(v); 
    } 

    // Move constructor 
    inline 
    Mat (Mat<T>&& m) 
    { 
        cols.swap(m.cols); 
    }

    // Constructs Matrix out of four vectors 
    inline 
    Mat (std::initializer_list< Vec<T> > l) : 
        cols{l} {} 

    // Copy-assignment operator 
    inline 
    Mat<T>& operator= (const Mat<T>& m) 
    { 
        for (auto& v : m.cols) 
            cols.push_back(v); 
        return *this; 
    } 

    // Move-assignment operator 
    inline 
    Mat<T>& operator= (Mat<T>&& m) 
    { 
        cols.swap(m.cols); 
        return *this; 
    }

    template<typename U> 
    inline 
    Mat<T>& operator= (const Mat<U>& m) 
    { 
        for (auto& v : m.cols) 
            cols.push_back(v); 
        return *this; 
    } 
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
