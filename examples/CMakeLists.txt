add_subdirectory (lib_renderer_app)

if(NOT DEFINED ANDROID)
	add_subdirectory (glfw_opengl_es_renderer)
endif()