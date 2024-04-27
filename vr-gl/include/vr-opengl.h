#pragma once

#if defined(_WIN32) || defined(__linux__)
	#define GLEW_STATIC
	#include <GL/glew.h>
#elif __ANDROID__
	#if __ANDROID_API__ >= 24
	#include <GLES3/gl32.h>
	#elif __ANDROID_API__ >= 21
	#include <GLES3/gl31.h>
	#else
	#include <GLES3/gl3.h>
	#endif
#endif
