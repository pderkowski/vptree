find_package(Git REQUIRED)

if((NOT CATCH_DIR) OR (NOT EXISTS ${CATCH_DIR}))
    set(CATCH_DIR ${CMAKE_SOURCE_DIR}/external/catch CACHE PATH "Path to Catch directory")

    execute_process(COMMAND git submodule update --init ${CATCH_DIR}
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

    set(CATCH_INCLUDE_DIR ${CATCH_DIR}/single_include CACHE INTERNAL "Path to include folder for Catch")
endif()

if((NOT PYBIND11_DIR) OR (NOT EXISTS ${PYBIND11_DIR}))
    set(PYBIND11_DIR ${CMAKE_SOURCE_DIR}/external/pybind11 CACHE PATH "Path to pybind11 directory")

    execute_process(COMMAND git submodule update --init ${PYBIND11_DIR}
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

add_subdirectory(${PYBIND11_DIR})
