cmake_minimum_required(VERSION 3.10) 

project(RayTracer20 C CXX) 

set_property(GLOBAL PROPERTY USE_FOLDERS ON) 

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /experimental:modules /std:c++latest /c /EHsc /MD") 

file(GLOB MODULES_SOURCES "*.ixx") 

message("${MODULES_SOURCES}") 

add_executable(RayTracer MODULES_SOURCES) 

set_target_properties(RayTracer PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin) 

install(TARGETS RayTracer RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/install) 
