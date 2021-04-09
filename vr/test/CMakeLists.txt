cmake_minimum_required (VERSION 3.8)

include (GoogleTest)

set(APP_NAME "vr_test")

set(SOURCES "gtest_smoke_test.cpp" "vk_layers.cpp" "vk_extensions.cpp")

add_executable(${APP_NAME} ${SOURCES})
target_compile_features(${APP_NAME} PRIVATE cxx_std_20)

target_link_libraries(${APP_NAME} vr gtest gtest_main)

gtest_discover_tests(${APP_NAME})
