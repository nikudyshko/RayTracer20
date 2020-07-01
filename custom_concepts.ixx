module; 

export module custom_concepts; 

import std.core; 

// Custom concepts for in-project use 

// Concept to ensure possibility of casting between two types 
export template<typename T, typename U> 
concept castable = requires (T t, U u) 
{ 
    { T(u) } -> T; 
    { U(t) } -> U; 
}; 

// Concept to ensure possibility of basic math between two types 
export template<typename T, typename U> 
concept mut_arithm = requires (T t, U u) 
{ 
    { t + u }; 
    { u + t }; 
    { t - u }; 
    { u - t }; 
    { t * u }; 
    { u * t }; 
    { t / u }; 
    { u / t }; 
}; 

// Concept to ensure possibility of basic math for single type 
export template<typename T> 
concept sim_arithm = requires (T a, T b) 
{ 
    { a++ }   -> T; 
    { ++a }   -> T; 
    { a-- }   -> T; 
    { --a }   -> T; 
    { a + b } -> T; 
    { a - b } -> T; 
    { a * b } -> T; 
    { a / b } -> T; 
}; 

// Concept to ensure applicability of complex math function to type 
export template<typename T> 
concept math_fun = requires(T v) 
{ 
    { std::abs(v) }; 
    { std::tan(v) }; 
    { std::sqrt(v) }; 
}; 

// Concept to ensure applicability of streaming operators 
export template<typename T> 
concept stream_ops = requires(T v) 
{ 
    { std::cin >> v }; 
    { std::cout << v }; 
}; 
