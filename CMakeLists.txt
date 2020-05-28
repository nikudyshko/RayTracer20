cmake_minimum_required(VERSION 3.10) 

project(RayTracer20) 

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_BUILD_TYPE release) 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /experimental:modules /std:c++latest") 

file(GLOB MODULES_SOURCES ".ixx") 
source_group("modules" FILES ${MODULES_SOURCES}) 

include_directories(.) 

add_executable(RayTracer20 Application.cpp) 

set_target_properties(RayTracer20 PROPERTIES RUNTIME_OUTPUT_DIRECTORY /bin) 

install(TARGETS RayTracer20 RUNTIME DESTINATION /install) 
