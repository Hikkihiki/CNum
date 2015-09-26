cmake_minimum_required (VERSION 2.6)

project (CNum)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_subdirectory(src)
add_subdirectory(thirdparty/googletest/googletest)
add_subdirectory(test)

enable_testing()
add_test(CNumTest test/CNumTest)
