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

####################################################################################################
# Dependencies
####################################################################################################

include(FetchContent)

FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        v2.4.3)
FetchContent_MakeAvailable(pybind11)

####################################################################################################
# Python module
####################################################################################################

# The Python module is compiled as a shared library, so all dependecies must be
# compiled with position independent code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(extern/rock)

pybind11_add_module(rock_python
    src/python_bindings.cpp)

set_target_properties(rock_python PROPERTIES OUTPUT_NAME rock)
target_link_libraries(rock_python PRIVATE rock)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(rock_python PUBLIC -fsized-deallocation)
endif()
