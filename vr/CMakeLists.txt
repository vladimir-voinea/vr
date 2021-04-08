cmake_minimum_required (VERSION 3.8)

find_package(Vulkan REQUIRED FATAL_ERROR)

set(LIB_NAME "vr")

set(SOURCES "src/glfw_initialization.cpp"
			"src/window.cpp" 
			"src/window_settings.cpp")
			
add_library (${LIB_NAME} ${SOURCES})
target_link_libraries(${LIB_NAME} PUBLIC glfw glm Vulkan::Vulkan)

target_include_directories(${LIB_NAME} PUBLIC include)
