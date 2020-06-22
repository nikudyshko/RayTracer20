module; 

export module shell; 

import std.core; 

import material; 
import surface; 

export template<typename T = float> 
class Shell 
{ 
private: 
	OpticalBulk<T> m_Prop; 
	std::vector< Surface<T> > m_Mesh; 
public: 
	Shell() {} 
}; 
