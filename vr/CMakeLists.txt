cmake_minimum_required (VERSION 3.8)

set(LIB_NAME "vr")

set(SOURCES "src/camera.cpp" "src/perspective_camera.cpp" "src/shader_material.cpp" "src/mesh.cpp" "src/scene.cpp" "src/texture.cpp" "src/object3d.cpp" "src/transformable.cpp" "src/cube_texture.cpp" "src/skybox.cpp" "src/orthographic_camera.cpp" "src/ambient_light.cpp" "src/directional_light.cpp" "src/point_light.cpp" "src/spot_light.cpp" "src/has_light_components.cpp" "src/has_light_attenuation.cpp" "src/has_intensity.cpp" "src/light.cpp")
			
add_library (${LIB_NAME} STATIC ${SOURCES})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
target_link_libraries(${LIB_NAME} PUBLIC vr-platform glm spdlog::spdlog assimp)

target_include_directories(${LIB_NAME} PUBLIC include)

enable_testing()
#add_subdirectory ("test")