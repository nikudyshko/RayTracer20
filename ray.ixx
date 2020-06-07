module; 

import vec; 

export module ray; 

template<typename T = float> 
class Ray 
{ 
private: 
	Vec<T> origin, dir; 
public: 
	Ray() = delete; 
	Ray(const Vec<T>& o, const Vec<T>& d) : 
		origin{o}, dir{d} {}; 
	Ray(std::initializaer_list<Vec<T>> l) 
	{ 
		assert(l.size() == 2); 
		auto it = l.first(); 
		origin = *it; 
		dir = *(it + 1); 
	}
}; 