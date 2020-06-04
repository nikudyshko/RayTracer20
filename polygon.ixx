module; 

import std.core; 
import vec; 

export module polygon; 

template<typename T = float> 
class Polygon 
{ 
private: 
	bool has_normal{false}; 
	Vec<T> normal; 
	std::vector<Vec<T>> vertices; 
public: 
	Polygon() = delete; 
	Polygon(const Vec<T>& p1, const Vec<T>& p2, const Vec<T>& p3) 
	{ 
		vertices.emplace_back(Vec<T>(p1)); 
		vertices.emplace_back(Vec<T>(p2)); 
		vertices.emplace_back(Vec<T>(p3)); 
	} 

	Vec<T> get_normal() 
	{ 
		if (has_normal) 
			return normal; 
		else 
		{ 
			Vec<T> v1 = p2 - p1; 
			Vec<T> v2 = p3 - p2; 
			normal = (v1 ^ v2).normalize(); 
			return normal; 
		}
	}
}; 
