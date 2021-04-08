cmake_minimum_required (VERSION 3.8)

set(APP_NAME "basic_window_app")

set(SOURCES main.cpp
			basic_window_app.cpp)

add_executable(${APP_NAME} ${SOURCES})

target_link_libraries(${APP_NAME} vr)
