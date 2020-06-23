module; 

export module surface; 

import vec; 
import polygon; 
import material; 

export template<typename T = float> 
class Surface 
{ 
private: 
    Polygon<T> m_Geom{}; 
    OpticalSurface<T> m_SurfOpt{};   
public: 
    Surface() {} 
    Surface(const Polygon<T>& p, 
            const OpticalSurface<T> s) : 
        m_Geom{p}, 
        m_SurfOpt{s} {}; 

    void set_polygon(const Polygon<T>& p) { m_Geom = Polygon<T>(p); } 
    void set_opt_prop(const OpticalSurface<T>& s) { m_SurfOpt = OpticalSurface<T>(s); } 
}; 
