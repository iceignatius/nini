cmake_minimum_required(VERSION 3.5)
project(NINI_Test)

find_library(HAVE_CMOCKA cmocka)
if(NOT HAVE_CMOCKA)
    message(FATAL_ERROR "Test program need CMOCKA library!")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

set(srcfiles ${srcfiles} ${PROJECT_SOURCE_DIR}/ninidump.c)
set(srcfiles ${srcfiles} ${PROJECT_SOURCE_DIR}/test_node.c)
set(srcfiles ${srcfiles} ${PROJECT_SOURCE_DIR}/test_decode.c)
set(srcfiles ${srcfiles} ${PROJECT_SOURCE_DIR}/test_encode.c)
set(srcfiles ${srcfiles} ${PROJECT_SOURCE_DIR}/test_helper.c)
set(srcfiles ${srcfiles} ${PROJECT_SOURCE_DIR}/test_cpp.cpp)
set(srcfiles ${srcfiles} ${PROJECT_SOURCE_DIR}/main.c)

set(deplibs ${deplibs} nini)
set(deplibs ${deplibs} cmocka)

add_executable(nini_test ${srcfiles})
target_link_libraries(nini_test ${deplibs})

# Copy test samples.
file(COPY ${CMAKE_CURRENT_SOURCE_FIR}samples DESTINATION ${CMAKE_BINARY_DIR})
