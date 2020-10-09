module; 

export module geometry; 

import std.core; 

import vec; 
import mat; 
import ray; 

export template<typename T> 
class Geometry 
{ 
public: 
    virtual Geometry<T>* get_copy() const { return nullptr; } 
    virtual void transform(const Mat<T> &matrix) {} 
    virtual Vec<T> get_normal(const Vec<T> &point) const { return {}; } 
    virtual std::vector< Vec<T> > get_coords() const { return {}; } 
    virtual bool ray_intersect(const Ray<T> &ray, T &sq_dist, Vec<T> &lx_point, Vec<T> &gx_point) const { return false; } 
}; 
