cmake_minimum_required (VERSION 3.8)

set(APP_NAME "vulkan_support_check_app")

set(SOURCES "main.cpp" "vulkan_support_check_app.cpp")

add_executable(${APP_NAME} ${SOURCES})
target_compile_features(${APP_NAME} PRIVATE cxx_std_20)

target_link_libraries(${APP_NAME} vr-glfw vr)
