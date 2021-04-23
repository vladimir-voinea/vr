if(NOT DEFINED BUILD_ANDROID)
	set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	add_subdirectory ("glfw")
endif()

add_subdirectory ("glm")

if(NOT DEFINED BUILD_ANDROID)
	add_subdirectory ("glew")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory ("gtest")

if(DEFINED BUILD_ANDROID)
	set(ASSIMP_ANDROID_JNIIOSYSTEM ON CACHE BOOL "" FORCE)
	set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
	set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
endif()
add_subdirectory ("assimp")

add_subdirectory ("spdlog")
