module; 

export module surface; 

import vec; 
import polygon; 
import material; 

export template<typename T = float> 
class Surface 
{ 
private: 
    Polygon<T> geom; 
    OpticalSurface<T> prop; 
}; 
