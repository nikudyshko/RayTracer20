import vec; 
import saveppm; 
import std.core; 

int main() 
{ 
	Vec<int> v0; 
	Vec<int> v1(1); 
	Vec<int> v2(1, 2, 3); 
	Vec<int> v3(1, 2, 3, 4); 
	Vec<int> v4(v1); 
	std::cout << v0 << '\n'; 
	/*std::cout << v0 << '\n' << v1 << '\n' << v2 << '\n' 
			  << v3 << '\n' << v4 << '\n'; */
	return 0; 
} 
