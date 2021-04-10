#pragma once

#ifdef _WIN32
	#define GLEW_STATIC
	#include <gl/glew.h>
#else
	#include <gl/GL.h>
#endif
