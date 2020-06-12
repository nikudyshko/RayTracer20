module; 

#include<cassert> 

import std.core; 
import vec; 

export module polygon; 

export template<typename T = float> 
class Polygon 
{ 
private: 
	bool has_normal{false}; 
	Vec<T> normal; 
	std::vector<Vec<T>> verts; 
public: 
	Polygon() = delete; 
	Polygon(const Vec<T>& p1, const Vec<T>& p2, const Vec<T>& p3) 
	{ 
		verts.emplace_back(Vec<T>(p1)); 
		verts.emplace_back(Vec<T>(p2)); 
		verts.emplace_back(Vec<T>(p3));  
	} 

	Vec<T> get_normal() 
	{ 
		if (has_normal) 
			return normal; 
		else 
		{ 
			Vec<T> v1 = verts[1] - verts[0]; 
			Vec<T> v2 = verts[2] - verts[0]; 

			has_normal = true; 
			normal = (v1 ^ v2).normalize(); 
			return normal; 
		} 
	} 
}; 
