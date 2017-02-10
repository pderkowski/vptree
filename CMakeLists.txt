cmake_minimum_required(VERSION 3.0)

project(VpTree)

add_library(VpTree INTERFACE)
target_include_directories(VpTree INTERFACE ${CMAKE_SOURCE_DIR})

add_subdirectory(test)

enable_testing()
add_test(NAME VpTreeTest COMMAND test_vptree)
