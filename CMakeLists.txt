cmake_minimum_required(VERSION 2.8.12)

project(VpTree)
set(CMAKE_BUILD_TYPE Release)

set(VPTREE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Path to VpTree include directory.")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_subdirectory(external)
add_subdirectory(python)

enable_testing()
add_subdirectory(test)