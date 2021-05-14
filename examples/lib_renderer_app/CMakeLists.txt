cmake_minimum_required (VERSION 3.8)

set(LIB_NAME "lib_renderer_app")

set(SOURCES "src/main_loop.cpp" "src/shader_loader.cpp" "src/parameters.cpp")

add_library (${LIB_NAME} STATIC ${SOURCES})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
target_link_libraries(${LIB_NAME} PUBLIC vr vr-gl vr-model vr-platform assimp)

target_include_directories(${LIB_NAME} PUBLIC include)
