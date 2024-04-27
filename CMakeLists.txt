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


if(NOT DEFINED ANDROID)
	find_package(OpenGL REQUIRED)
	if(WIN32)
		set(OPENGL_LIBS ${OPENGL_gl_LIBRARY} glew_lib::static)
	else()
		find_package(PkgConfig)
		pkg_check_modules(GLEW glew)
		find_package(GLEW REQUIRED)
		set(OPENGL_LIBS ${OPENGL_gl_LIBRARY} GLEW::GLEW)
	endif()
else()
	set(OPENGL_LIBS ${OPENGL_LIB} EGL)
endif()

# External dependencies
add_subdirectory ("dependencies")

add_subdirectory ("vr")
add_subdirectory ("vr-model")
add_subdirectory ("vr-gl")
add_subdirectory ("vr-platform")

if(NOT DEFINED ANDROID)
	add_subdirectory ("vr-glfw")
endif()

if(NOT DEFINED ANDROID)
	# CMake modules
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#add_subdirectory ("vr-vk")
endif()

add_subdirectory ("examples")

message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
