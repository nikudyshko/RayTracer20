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

	Polygon(std::initializer_list<Vec<T>> l) 
	{ 
		assert(l.size() == 4); 
		vertices = l; 
	} 

	Vec<T> get_normal() 
	{ 
		if (has_normal) 
			return normal; 
		else 
		{ 
			Vec<T> v1 = vertices[1] - vertices[0]; 
			Vec<T> v2 = vertices[2] - vertices[1]; 

			has_normal = true; 
			normal = (v1 ^ v2).normalize(); 
			return normal; 
		} 
	} 
}; 
