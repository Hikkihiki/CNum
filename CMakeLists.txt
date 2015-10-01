cmake_minimum_required (VERSION 2.6)

project (CNum)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

if(MSVC)
  add_definitions(/WX /W4 /D_SCL_SECURE_NO_WARNINGS)
endif()

set(gtest_force_shared_crt ON CACHE BOOL "")
add_subdirectory(thirdparty/googletest/googletest)

include_directories(include)
add_subdirectory(src)

add_subdirectory(test)

enable_testing()
add_test(CNumTest test/CNumTest)
