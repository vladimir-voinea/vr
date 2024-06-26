cmake_minimum_required (VERSION 3.8)

set(APP_NAME "glfw_opengl_es_renderer_app")

set(SOURCES "main.cpp" "glfw_opengl_es_renderer_app.cpp" "fps_counter.cpp" "input_listener.cpp" "preamble.cpp" "gui.cpp" "parameters_persistency.cpp")

add_executable(${APP_NAME} ${SOURCES})
target_compile_features(${APP_NAME} PRIVATE cxx_std_17)

if(WIN32)
    set(glewlib glew_lib::static)
else()
    set(glewlib GLEW::GLEW)
endif()

target_link_libraries(${APP_NAME} vr vr-glfw vr-platform ${glewlib} lib_renderer_app imgui cereal)

set(DATA_FOLDER_IN "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(DATA_FOLDER_OUT "${CMAKE_CURRENT_BINARY_DIR}/data")

set(COPY_TARGET_NAME "${APP_NAME}-copy-data")

add_custom_target(${COPY_TARGET_NAME} ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${DATA_FOLDER_IN} ${DATA_FOLDER_OUT})
