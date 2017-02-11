add_executable(test_vptree test_vptree.cpp)
set_property(TARGET test_vptree PROPERTY CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

target_link_libraries(test_vptree Catch VpTree)
