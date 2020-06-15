module; 

export module shell; 

import std.core; 

import surface; 

export template<typename T = float> 
class Shell 
{ 
private: 
	std::vector< Surface<T> > mesh; 
public: 
	Shell() {} 
}; 
