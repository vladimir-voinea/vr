cmake_minimum_required (VERSION 3.8)

find_package(Vulkan REQUIRED FATAL_ERROR)

set(LIB_NAME "vr")

set(SOURCES "src/vk_instance.cpp" "src/vk_extensions.cpp" "src/vk_layers.cpp" "src/vk_debug_default_callback.cpp" "include/vk_validation_layer.hpp" "src/vk_validation_layer.cpp")
			
add_library (${LIB_NAME} ${SOURCES})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
target_link_libraries(${LIB_NAME} PUBLIC glm Vulkan::Vulkan)

target_include_directories(${LIB_NAME} PUBLIC include)

enable_testing()
add_subdirectory ("test")