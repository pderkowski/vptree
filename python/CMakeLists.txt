pybind11_add_module(vptree pyvptree.cpp)
target_include_directories(vptree PRIVATE ${VPTREE_INCLUDE_DIR})
set(PYVPTREE_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "Path to python extension")