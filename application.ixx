module; 

import vec; 
import polygon; 
import saveppm; 
import std.core; 

export module application; 

const int width = 1024;  
const int height = 768; 

export int main() 
{ 
	std::vector<Vec<float>> pics(width*height); 
	for (int j = 0; j < height; ++j) 
		for (int i = 0; i < width; ++i) 
			pics[j*width + i] = { j / float(height), i / float(width), 0.0f }; 
	save_ppm<float>(width, height, pics); 
	Vec<float> v1{1, 2, 4}; 
	Vec<float> v2{4, 5, 3}; 
	Vec<float> v3{7, 8, 9}; 
	Polygon<float> p{v1, v2, v3}; 
	Vec<float> norm = p.get_normal(); 
	std::cout << norm << '\n'; 
	return 0; 
} 
