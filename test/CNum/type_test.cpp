#include "CNum/type.hpp"
#include "gtest/gtest.h"
using ::testing::Test;

TEST(CNumTest, Equals) {
	CNum::Int a(10L);
	ASSERT_TRUE(a == 10);
	ASSERT_TRUE(a == 10L);

	ASSERT_FALSE(a == 11);
	ASSERT_FALSE(a == 9L);
}

TEST(CNumTest, NotEquals) {
	CNum::Int a(1);
	ASSERT_FALSE(a != 1);
	ASSERT_FALSE(a != 1L);

	ASSERT_TRUE(a != 0);
	ASSERT_TRUE(a != -1L);
}

