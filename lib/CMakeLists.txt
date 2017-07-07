cmake_minimum_required(VERSION 3.5)
project(NINI_Library)

include_directories(${CMAKE_SOURCE_DIR}/include)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

set(srcfiles ${srcfiles} ${CMAKE_SOURCE_DIR}/src/nini_node.c)
set(srcfiles ${srcfiles} ${CMAKE_SOURCE_DIR}/src/nini_root.c)
set(srcfiles ${srcfiles} ${CMAKE_SOURCE_DIR}/src/nini_helper.c)

add_library(nini ${srcfiles})

install(TARGETS nini DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include/nini)
