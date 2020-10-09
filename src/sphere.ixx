module; 

export module sphere; 

import std.core; 

import geometry; 

export template<typename T> 
class Sphere : public Geometry<T> 
{ 
private: 
    // Radius 
    T m_Radius{}; 
    // Center point and a point on a Sphere 
    Vec<T> m_Center{}, m_Sphere{}; 
public: 
    // Default constructor 
    Sphere () {} 
    // Constructs sphere from center point and a point on a Sphere 
    Sphere (const Vec<T> &c, const Vec<T> &s) : 
        m_Center{c}, 
        m_Sphere{s} { m_Radius = (m_Sphere - m_Center).length(); } 

    // Returns a copy of Sphere 
    Geometry<T>* get_copy() const override 
    { 
        return new Sphere<T>{m_Center, m_Sphere}; 
    }

    // Apply a matrix transform to a Sphere (not scaling!!!) 
    void transform (const Mat<T> &matrix) override 
    { 
        m_Center.w = T(1); 
        m_Center = matrix*m_Center; 
        m_Center.w = T(0); 
        m_Sphere.w = T(1); 
        m_Sphere = matrix*m_Sphere; 
        m_Sphere.w = T(0); 

        m_Radius = (m_Sphere - m_Center).length(); 
    } 

    // Returns the normal of Sphere at a given point 
    Vec<T> get_normal(const Vec<T> &point) const override 
    { 
        return (point - m_Center).normalize(); 
    } 

    // Returns the coordinates of Sphere 
    std::vector< Vec<T> > get_coords() const override 
    { return { m_Center, m_Sphere }; } 

    // Code to check if the Ray intersects the Sphere 
    // ray - ray to check, sq_dist - squared distance to hit point, lx_point - local coordinates of hit point, 
    // gx_point - global coordinates of hit point 
    bool ray_intersect (const Ray<T> &ray, T &dist, Vec<T> &lx_point, Vec<T> &gx_point) const override 
    { 
        Vec<T> L = ray.origin - m_Center; 

        T b = T(2)*ray.dir*L; 
        T c = L*L - m_Radius*m_Radius; 

        if (T d = b*b - T(4)*c; d >= T(0)) 
        { 
            T sd = std::sqrt(d); 
            T t1 = (-b - sd)/T(2); 
            T t2 = (-b + sd)/T(2); 

            if ((t1 < T(0)) && (t2 < T(0))) 
                return false; 

            T t = std::min(std::max(t1, T(0)), std::max(t2, T(0))); 

            lx_point = {}; 

            gx_point = ray.origin + t * ray.dir; 

            dist = t; 

            return true; 
        } 
        return false; 
    }
}; 
