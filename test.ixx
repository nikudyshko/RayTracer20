module; 

export module test; 

import std.core; 

import vec; 
import mat; 

export template<typename T> 
Mat<T> lookAt(const Vec<T>& from, const Vec<T>& to, Vec<T>& tmp = Vec<T>{0, 1, 0}) 
{ 
    Vec<T> forward = (from - to).normalize();  
    Vec<T> right = tmp.normalize() ^ forward; 
    Vec<T> up = forward ^ right; 

    std::cout << forward << '\n' << up << '\n' << right << '\n'; 

    Mat<T> camToWorld = Mat<T>{ 
        Vec<T>{ right.x, up.x, forward.x, T(0) }, 
        Vec<T>{ right.y, up.y, forward.y, T(0) }, 
        Vec<T>{ right.z, up.z, forward.z, T(0) }, 
        Vec<T>{ from.x, from.y, from.z, T(1) } 
    }; 

    return camToWorld; 
} 
