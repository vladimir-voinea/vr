cmake_minimum_required (VERSION 3.8)

set(LIB_NAME "vr-glfw")

set(SOURCES "src/glfw_initialization.cpp" "src/glfw_window.cpp" "src/glfw_window_settings.cpp" "src/glfw_util.cpp"   "include/glfw_keyboard.hpp" "include/glfw_mouse.hpp" "src/glfw_mouse.cpp" "src/glfw_keyboard.cpp")
			
add_library (${LIB_NAME} ${SOURCES})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
target_link_libraries(${LIB_NAME} PUBLIC glfw spdlog::spdlog)

target_include_directories(${LIB_NAME} PUBLIC include)
