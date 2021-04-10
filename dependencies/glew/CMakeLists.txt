cmake_minimum_required (VERSION 3.8)

set(IMPORTED_NAME "glew_lib::static")
set(LIB_FILE "${CMAKE_CURRENT_LIST_DIR}/glew-2.1.0/lib/Release/x64/glew32s.lib")
set(LIB_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/glew-2.1.0/include")

message(STATUS "Created global target ${IMPORTED_NAME}")
message(STATUS "${IMPORTED_NAME} library path: ${LIB_FILE}")
message(STATUS "${IMPORTED_NAME} include directory: ${LIB_INCLUDE_DIR}")

add_library(${IMPORTED_NAME} STATIC IMPORTED GLOBAL)
set_target_properties(${IMPORTED_NAME} PROPERTIES
    IMPORTED_LOCATION "${LIB_FILE}"
    INTERFACE_INCLUDE_DIRECTORIES "${LIB_INCLUDE_DIR}"
    )
