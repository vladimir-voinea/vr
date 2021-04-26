cmake_minimum_required (VERSION 3.8)

if(NOT DEFINED ANDROID)
	set(PLATFORM_SOURCES "src/pc_platform_manager.cpp" "src/pc_asset_manager.cpp")
	set(LIBS "")
else()
	set(PLATFORM_SOURCES "src/ndk_interface.cpp" "src/android_platform_manager.cpp" "src/android_asset_manager.cpp")
	set(LIBS android)
endif()

set(BASE_SOURCES "src/asset.cpp" "src/platform_manager_factory.cpp")

set(LIB_NAME "vr-platform")
			
add_library (${LIB_NAME} SHARED ${BASE_SOURCES} ${PLATFORM_SOURCES})
target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
target_link_libraries(${LIB_NAME} PUBLIC ${LIBS})

target_include_directories(${LIB_NAME} PUBLIC include)

enable_testing()