cmake_minimum_required (VERSION 3.8)

set(LIB_NAME "vr")

set(SOURCES "src/camera.cpp" "src/perspective_camera.cpp" "src/mesh.cpp" "src/scene.cpp" "src/texture.cpp" "src/object3d.cpp")
			
add_library (${LIB_NAME} ${SOURCES})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
target_link_libraries(${LIB_NAME} PUBLIC glm)

target_include_directories(${LIB_NAME} PUBLIC include)

enable_testing()
add_subdirectory ("test")