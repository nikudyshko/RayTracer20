export module saveppm; 

#include <iostream> 
#include <fstream> 

export void save_ppm( 
	size_t width, 
	size_t height, 
	const std::vector<Vec3f>& framebuffer
) 
{ 
	std::ofstream output; 
	output.open("./render.ppm"); 
	output << "P6\n" << width << " " << height << "\n255\n"; 
	for (size_t i = 0; i < width * height; ++i) 
		for (size_t j = 0; j < 3; ++j) 
			output << static_cast<char>(255 * std::max(0.f, std::min(1.f, framebuffer[i][j]))); 
	output.close(); 
}
