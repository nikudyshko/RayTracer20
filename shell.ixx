module; 

export module shell; 

import std.core; 

import material; 
import surface; 

export template<typename T = float> 
class Shell 
{ 
private: 
	OpticalBulk<T> prop; 
	std::vector< Surface<T> > mesh; 
public: 
	Shell() {} 
}; 
