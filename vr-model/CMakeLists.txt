cmake_minimum_required (VERSION 3.8)

set(LIB_NAME "vr-model")

set(SOURCES "src/model_loading.cpp")
			
add_library (${LIB_NAME} STATIC ${SOURCES})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
target_link_libraries(${LIB_NAME} PUBLIC vr vr-gl vr-platform glm spdlog::spdlog assimp)

target_include_directories(${LIB_NAME} PUBLIC include)

enable_testing()
#add_subdirectory ("test")