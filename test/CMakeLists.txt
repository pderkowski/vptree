add_executable(test_vptree test_vptree.cpp)
set_property(TARGET test_vptree PROPERTY CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

target_link_libraries(test_vptree Catch VpTree)
add_test(NAME VpTree_C++_Tests COMMAND test_vptree)

find_package(PythonInterp REQUIRED)
add_test(NAME VpTree_Python_Tests COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test_pyvptree.py ${PYVPTREE_DIR})