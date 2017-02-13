add_executable(test_vptree test_vptree.cpp)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

target_include_directories(test_vptree PRIVATE ${VPTREE_INCLUDE_DIR} PRIVATE ${CATCH_INCLUDE_DIR})

add_test(NAME VpTree_C++_Tests COMMAND test_vptree)

# find_package(PythonInterp REQUIRED)
add_test(NAME VpTree_Python_Tests COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test_pyvptree.py ${PYVPTREE_DIR})