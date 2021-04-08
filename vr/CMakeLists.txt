# CMakeList.txt : CMake project for vr, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

find_package(Vulkan REQUIRED FATAL_ERROR)

# Add source to this project's executable.
add_executable (vr "vr.cpp" "vr.h")
target_link_libraries(vr PRIVATE glfw glm Vulkan::Vulkan)

# TODO: Add tests and install targets if needed.
