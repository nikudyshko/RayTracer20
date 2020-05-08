export module vec; 

template<typename T, size_t size = 4> 
class Vec 
{ 
public: 
	union 
	{ 
		T arr_[size]; 
		T x, y, z, w; 
		T r, g, b, a; 
	}
}; 
