#include "CNum/type.hpp"
#include "gtest/gtest.h"
using ::testing::Test;
using ::CNum::Z;

TEST(CNumZTest, Assignment) {
	Z a = 10;
	ASSERT_TRUE(a == 10);
	a = 20;
	ASSERT_TRUE(a == 20);
	a = 30L;
	ASSERT_TRUE(a == 30);
	a = Z(40);
	ASSERT_TRUE(a == 40);
}

TEST(CNumZTest, Equals) {
	Z a(10L);
	ASSERT_TRUE(a == 10);
	ASSERT_TRUE(a == 10L);

	ASSERT_FALSE(a == 11);
	ASSERT_FALSE(a == 9L);

	ASSERT_TRUE(a == Z(10));
	ASSERT_FALSE(a == Z(9));
}

TEST(CNumZTest, NotEquals) {
	Z a(1);
	ASSERT_FALSE(a != 1);
	ASSERT_FALSE(a != 1L);

	ASSERT_TRUE(a != 0);
	ASSERT_TRUE(a != -1L);

	ASSERT_TRUE(a != Z(2));
	ASSERT_FALSE(a != Z(1L));
}

