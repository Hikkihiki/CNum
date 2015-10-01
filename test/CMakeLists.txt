set(GTEST_DIR "${PROJECT_SOURCE_DIR}/thirdparty/googletest/googletest")
include_directories("${GTEST_DIR}/include" "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/include")

#set(CMAKE_LIBRARY_PATH "${GTEST_DIR}/bin")
#link_directories("${GTEST_DIR}")
#find_library(GTEST_LIB NAMES gtest gtest_main)

add_executable(CNumTest cnum_test.cpp CNum/type_test.cpp CNum/counter_test.cpp)
target_link_libraries(CNumTest gtest cnum)
set_property(TARGET CNumTest PROPERTY CXX_STANDARD 11)

