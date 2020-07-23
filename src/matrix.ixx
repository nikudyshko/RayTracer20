module; 

export module mat;  

import std.core; 

import vec; 

// Struct to implement 4*4 matrix 
export template<typename T> 
struct Mat 
{ 
    // Matrix is represented as vector of row vectors (for now) 
    std::vector< Vec<T> > rows{}; 

    // Default constructor 
    inline 
    Mat () {} 

    // Copy constructor 
    inline 
    Mat (const Mat<T>& m) 
    { 
        for (auto& v : m.rows) 
            rows.push_back(v); 
    } 

    // Move constructor 
    inline 
    Mat (Mat<T>&& m) 
    { 
        rows.swap(m.rows); 
    }

    // Constructs Matrix out of four vectors 
    inline 
    Mat (std::initializer_list< Vec<T> > l) : 
        rows{l} {} 

    // Copy-assignment operator 
    inline 
    Mat<T>& operator= (const Mat<T>& m) 
    { 
        for (auto& v : m.rows) 
            rows.push_back(v); 
        return *this; 
    } 

    // Move-assignment operator 
    inline 
    Mat<T>& operator= (Mat<T>&& m) 
    { 
        rows.swap(m.rows); 
        m.rows.clear(); 
        return *this; 
    }

    template<typename U> 
    inline 
    Mat<T>& operator= (const Mat<U>& m) 
    { 
        for (auto& v : m.rows) 
            rows.push_back(v); 
        return *this; 
    } 
}; 

// Matrix-Vector multiplication 
export template<typename T, typename U> 
auto operator* (const Mat<T>& m, const Vec<U>& v) -> Vec<decltype(m.rows[0].x*v.x)> 
{ 
    using W = decltype(m.rows[0].x*v.x); 

    Vec<W> res{}; 

    for (size_t i = 0; i < m.rows.size(); ++i) 
        res[i] = m.rows[i]*v; 

    return res; 
}
