cmake_minimum_required (VERSION 3.8)

if(NOT DEFINED ANDROID)
	find_package(OpenGL REQUIRED)
	set(OPENGL_LIBS opengl32 glew_lib::static)
else()
	set(OPENGL_LIBS ${OPENGL_LIB} EGL)
endif()

set(LIB_NAME "vr-gl")

set(SOURCES "src/shader.cpp" "src/shader_program.cpp" "src/renderer_cache.cpp" "src/renderer.cpp" "src/opengl_shader_material.cpp" "src/texture_loader.cpp" "src/opengl_shader.cpp" "src/stb_image.h" "src/opengl_debug_callback.cpp" "src/color_material.cpp" "src/cube_texture_material.cpp")
			
add_library (${LIB_NAME} STATIC ${SOURCES})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
target_link_libraries(${LIB_NAME} PUBLIC vr glm ${OPENGL_LIBS})

target_include_directories(${LIB_NAME} PUBLIC include)

enable_testing()