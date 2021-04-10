cmake_minimum_required (VERSION 3.8)

find_package(OpenGL REQUIRED)

set(LIB_NAME "vr-ogl")

set(SOURCES "src/test.cpp")
			
add_library (${LIB_NAME} ${SOURCES})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
target_link_libraries(${LIB_NAME} PUBLIC glm opengl32)

target_include_directories(${LIB_NAME} PUBLIC include)

enable_testing()