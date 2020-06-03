import vec; 
import saveppm; 
import std.core; 

int main() 
{ 
	Vec<int> v1(1, 2, 3); 
	Vec<float> v2(4.0f, 5.0f, 6.0f); 
	auto v3 = v1 ^ v2; 
	std::cout << v1[0] << ' ' << v1[1] << ' ' << v1[2] << ' ' << v1[3] << '\n';  
	return 0; 
} 
