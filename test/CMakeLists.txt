set(GTEST_DIR "${PROJECT_SOURCE_DIR}/../googletest/googletest")

include_directories("${GTEST_DIR}/include")
#link_directories("${GTEST_DIR}/lib")
#set(CMAKE_LIBRARY_PATH "${GTEST_DIR}/bin")

FIND_LIBRARY(GTEST_LIB NAMES gtest gtest_main PATHS "${GTEST_DIR}/lib")

add_executable(CNumTest cnum_test.cpp)
target_link_libraries(CNumTest ${GTEST_LIB})

