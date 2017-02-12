cmake_minimum_required(VERSION 3.0)

project(VpTree)
set(CMAKE_BUILD_TYPE Release)

add_library(VpTree INTERFACE)
target_include_directories(VpTree INTERFACE ${CMAKE_SOURCE_DIR})
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_subdirectory(external)
add_subdirectory(python)

enable_testing()
add_subdirectory(test)