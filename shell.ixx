module; 

export module shell; 

import std.core; 

import polygon; 

export template<typename T = float> 
class Shell 
{ 
private: 
	std::vector< Polygon<T> > mesh; 
public: 
	Shell() {} 
}; 
