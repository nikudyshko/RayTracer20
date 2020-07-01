module; 

export module saveppm; 

import std.core; 

import vec; 

// Function that saves image in .ppm format 
export template<typename T = float> 
void save_ppm( 
	size_t width, 		// Image width 
	size_t height, 		// Image height 
	const std::vector< Vec<T> >& framebuffer // Flattened array of pixels 
) 
{ 
	std::ofstream output; 
	output.open("./render.ppm", std::ios::out || std::ios::binary); 
	output << "P6\n" << width << ' ' << height << "\n255\n"; 
	for (size_t i = 0; i < width*height; ++i) 
		for (size_t j = 0; j < 3; ++j) 
			output << char(255*std::max(0.f, std::min(1.f, framebuffer[i][j])));  
	output.close(); 
}
