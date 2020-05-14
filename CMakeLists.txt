cmake_minimum_required(VERSION 3.14)

project(rock_python
    VERSION 0.0.0
    DESCRIPTION "Romanian checkers Python bindings"
    LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

include(FetchContent)

set(BUILD_SHARED_LIBS TRUE)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

add_subdirectory(extern/rock)
add_subdirectory(src)
