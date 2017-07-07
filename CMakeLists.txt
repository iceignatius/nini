cmake_minimum_required(VERSION 3.5)
project(NINI)

find_library(HAVE_CMOCKA cmocka)
message("cmocka: ${HAVE_CMOCKA}")

add_subdirectory(lib)

if(HAVE_CMOCKA)
    enable_testing()
    add_subdirectory(test)
    add_test(unit_tests ${CMAKE_BINARY_DIR}/test/nini_test)
endif()
