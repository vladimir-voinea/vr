cmake_minimum_required (VERSION 3.8)

set(APP_NAME "glfw_opengl_es_cube_app")

set(SOURCES "main.cpp" "glfw_opengl_es_cube_app.cpp" "main_loop.cpp" "shader_loader.cpp")

add_executable(${APP_NAME} ${SOURCES})
target_compile_features(${APP_NAME} PRIVATE cxx_std_20)

target_link_libraries(${APP_NAME} vr-glfw vr-gl glm)
