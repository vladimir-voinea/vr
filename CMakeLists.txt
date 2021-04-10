# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
project(vr VERSION 0.1 DESCRIPTION "Vulkan graphics library")

# CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# External dependencies
add_subdirectory ("dependencies")

# vr-glfw directory
add_subdirectory ("vr-glfw")

# vr-gl directory
add_subdirectory ("vr-gl")

# vr-vk directory
add_subdirectory ("vr-vk")

# examples directory
add_subdirectory ("examples")