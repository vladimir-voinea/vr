cmake_minimum_required (VERSION 3.8)

set(APP_NAME "glfw_opengl_es_controls")

set(SOURCES "main.cpp" "glfw_opengl_es_controls_app.cpp" "main_loop.cpp" "shader_loader.cpp" "controls.cpp" "controls.hpp")

add_executable(${APP_NAME} ${SOURCES})
target_compile_features(${APP_NAME} PRIVATE cxx_std_20)

target_link_libraries(${APP_NAME} vr-glfw vr-gl glm)

set(DATA_FOLDER_IN "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(DATA_FOLDER_OUT "${CMAKE_CURRENT_BINARY_DIR}/data")

add_custom_target(copy-data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${DATA_FOLDER_IN} ${DATA_FOLDER_OUT})
