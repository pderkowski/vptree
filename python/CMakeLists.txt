pybind11_add_module(vptree pyvptree.cpp)
target_link_libraries(vptree VpTree)
set(PYVPTREE_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "Path to python extension")