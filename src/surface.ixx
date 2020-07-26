module; 

export module surface; 

import vec; 
import polygon; 
import material; 

// Struct to implement basic element of surface 
export template<typename T> 
class Surface 
{ 
private: 
    // ID given by Shell  
    size_t m_SurfaceID{0}; 
    // m_Geom - geometrical properties of Surface
    Polygon<T> m_Geom{}; 
    // m_SurfOpt - optical properties of Surface 
    OpticalSurface<T> m_SurfOpt{};   
public: 
    // Default constructor 
    Surface() {} 
    // Constructs Surface from Polygon and Optical properties 
    Surface(const Polygon<T>& p, 
            const OpticalSurface<T> s) : 
        m_Geom{p}, 
        m_SurfOpt{s} {}; 

    inline 
    void set_surface_id(size_t id) noexcept 
    { m_SurfaceID = id; } 

    inline 
    size_t get_surface_id() const noexcept 
    { return m_SurfaceID; } 

    // Sets the geometry of Surface 
    void set_polygon(const Polygon<T>& p) { m_Geom = Polygon<T>(p); } 
    // Sets the optical properties of Surface 
    void set_opt_prop(const OpticalSurface<T>& s) { m_SurfOpt = OpticalSurface<T>(s); } 

    // Returns the geometry of Surface  
    const Polygon<T>& get_polygon() const noexcept { return m_Geom; } 
    // Returns the Surface optic properties of a given coordinates 
    OpticalSurface<T> get_surf_opt(const Vec<T>& coords) const noexcept { return m_SurfOpt; }
}; 
