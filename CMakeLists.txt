# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
project(vr VERSION 0.1 DESCRIPTION "Vladimir's graphics library")

if(DEFINED ANDROID)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frtti -fexceptions -Wall")
endif()

if(${ANDROID_PLATFORM_LEVEL})
	set(BUILD_ANDROID ON CACHE BOOL "" FORCE)
endif()


# External dependencies
add_subdirectory ("dependencies")

# vr-glfw directory
add_subdirectory ("vr")

if(NOT DEFINED BUILD_ANDROID)
	# vr-glfw directory
	add_subdirectory ("vr-glfw")
endif()

# vr-gl directory
add_subdirectory ("vr-gl")

if(NOT DEFINED BUILD_ANDROID)
	# CMake modules
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
	# vr-vk directory
add_subdirectory ("vr-vk")
endif()

if(NOT DEFINED BUILD_ANDROID)
	# examples directory
	add_subdirectory ("examples")
endif()
message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
