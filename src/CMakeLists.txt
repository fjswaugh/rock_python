FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        v2.4.3)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(rock_python
    python_bindings.cpp)
set_target_properties(rock_python PROPERTIES OUTPUT_NAME rock)
target_link_libraries(rock_python PRIVATE rock)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    target_compile_options(rock_python PUBLIC -fsized-deallocation)
endif()
