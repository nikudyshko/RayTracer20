module; 

export module surface; 

import vec; 
import mat; 
import geometry; 
import material; 

// Struct to implement basic element of surface 
export template<typename T> 
class Surface 
{ 
private: 
    // ID given by Shell  
    size_t m_SurfaceID{0}; 
    // m_Geom - geometrical properties of Surface 
    Geometry<T> *m_Geom{nullptr}; 
    // m_SurfOpt - optical properties of Surface 
    OpticalSurface<T> m_SurfOpt{};   
public: 
    // Default constructor 
    Surface() {} 
    // Constructs Surface from Polygon and Optical properties 
    Surface(Geometry<T> *g, 
            const OpticalSurface<T> s) : 
        m_Geom{g}, 
        m_SurfOpt{s} {}; 

    Surface(const Surface<T> &s) 
    {   
        m_Geom = s.m_Geom->get_copy(); 
        m_SurfOpt = s.m_SurfOpt; 
    }

    ~Surface() 
    { 
        delete m_Geom; 
    } 

    Surface<T> operator=(const Surface<T> &s) 
    {  
        delete m_Geom; 
        m_Geom = s.m_Geom->get_copy(); 
        m_SurfOpt = s.m_SurfOpt; 
    }

    inline 
    void set_surface_id(size_t id) noexcept 
    { m_SurfaceID = id; } 

    inline 
    size_t get_surface_id() const noexcept 
    { return m_SurfaceID; } 

    // Sets the geometry of Surface 
    void set_geometry(Geometry<T> *g) { m_Geom = g; } 
    // Sets the optical properties of Surface 
    void set_opt_prop(const OpticalSurface<T>& s) { m_SurfOpt = OpticalSurface<T>(s); } 

    // Apply a matrix transform to Polygon 
    void transform(const Mat<T>& matrix) 
    { m_Geom->transform(matrix); } 

    // Returns the geometry of Surface  
    const Geometry<T>* get_geometry() const noexcept { return m_Geom; } 
    // Returns the Surface optic properties of a given coordinates 
    OpticalSurface<T> get_surf_opt(const Vec<T>& coords) const noexcept { return m_SurfOpt; }
}; 
