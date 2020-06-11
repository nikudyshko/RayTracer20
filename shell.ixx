module; 

import std.core; 

export module shell; 

export template<typename T = float> 
class Shell 
{ 
private: 
	std::vector<Polygon<T>> mesh; 
public: 
	Shell() {} 
}; 
