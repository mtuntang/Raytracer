# CMakeList.txt : CMake project for Raytracer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Raytracer")

# Add source to this project's executable.
add_executable (Raytracer   "main.cpp" "vec3.h" "ray.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Raytracer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

