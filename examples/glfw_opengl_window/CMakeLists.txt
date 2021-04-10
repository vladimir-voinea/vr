cmake_minimum_required (VERSION 3.8)

set(APP_NAME "glfw_opengl_window_app")

set(SOURCES "main.cpp" "glfw_opengl_window_app.cpp")

add_executable(${APP_NAME} ${SOURCES})
target_compile_features(${APP_NAME} PRIVATE cxx_std_20)

target_link_libraries(${APP_NAME} vr-glfw)
