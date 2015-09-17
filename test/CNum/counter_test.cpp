#include "../../include/counter.hpp"

#include "gtest/gtest.h"
using ::testing::Test;
using ::CNum::Counter;

TEST(CNumCounter, DefaultConstructor) {
	{
		Counter a;
		ASSERT_TRUE(a == 0);
	}
}

TEST(CNumCounter, ExpicitConstructor) {
	{
		Counter a(1);
		ASSERT_TRUE(a == 1);
	}
	{
		Counter a(0);
		ASSERT_TRUE(a == 0);
	}
	{
		Counter a(-1);
		ASSERT_TRUE(a == -1);
	}
	{
		Counter a(192837465L);
		ASSERT_TRUE(a == 192837465L);
	}
}

TEST(CNumCounter, CopyConstructor) {
	{
		Counter a = Counter(10);
		ASSERT_TRUE(a == 10);
	}
	{
		Counter a = -10;
		ASSERT_TRUE(a == -10);
	}
	{
		Counter a = -0;
		ASSERT_TRUE(a == 0);
	}
}

TEST(CNumCounter, CopyAssignment) {
	Counter a;
	ASSERT_TRUE(a == 0);
	a = 20;
	ASSERT_TRUE(a == 20);
	a = 30L;
	ASSERT_TRUE(a == 30);
	a = -20;
	ASSERT_TRUE(a == -20);
}

TEST(CNumCounter, PrefixIncrement) {
	Counter a = 0;
	unsigned long ul = 0;
	ASSERT_TRUE(++ul == 1);
	ASSERT_TRUE(++a == 1);
	ASSERT_TRUE(++ul == 2);
	ASSERT_TRUE(++a == 2);
	a = 0;
	ul = 0;

	// bottleneck at == comparision;
	for (unsigned long i = 0; i < 999999; i++) {
		ASSERT_TRUE(++ul == i + 1);
		ASSERT_TRUE(++a == i + 1);
	}
}

TEST(CNumCounter, PostfixIncrement) {
	Counter a = 0;
	unsigned long ul = 0;

	ASSERT_TRUE(ul++ == 0);
	ASSERT_TRUE(a++ == 0);
	ASSERT_TRUE(ul++ == 1);
	ASSERT_TRUE(a++ == 1);
	ASSERT_TRUE(ul == 2);
	ASSERT_TRUE(a == 2);
	a = 999999;
	ul = 999999;
	// bottleneck at == comparision;
	for (unsigned long i = 999999; i < 2 * 999999; i++) {
		ASSERT_TRUE(ul++ == i);
		ASSERT_TRUE(a++ == i);
	}
}

TEST(CNumCounter, PrefixDecrement) {
	{
		Counter a = 1;
		unsigned long ul = 1;
		ASSERT_TRUE(--ul == 0);
		ASSERT_TRUE(--a == 0);
		ASSERT_TRUE(--ul == -1UL);
		ASSERT_DEATH(--a, "");
	}
	{
		Counter a = 999999;
		unsigned long ul = 999999;
		// bottleneck at == comparision;
		for (unsigned long i = 999999; i > 0; --i) {
			ASSERT_TRUE(--ul == i - 1);
			ASSERT_TRUE(--a == i - 1);
		}
	}
}

TEST(CNumCounter, PostfixDecrement) {
	{
		Counter a = 1;
		unsigned long ul = 1;
		ASSERT_TRUE(ul-- == 1);
		ASSERT_TRUE(a-- == 1);
		ASSERT_TRUE(ul-- == 0);
		ASSERT_DEATH(a--, "");
	}
	{
		Counter a = 2 * 999999;
		Counter ul = 2 * 999999;
		// bottleneck at == comparision;
		for (unsigned long i = 2 * 999999; i > 999999; --i) {
			ASSERT_TRUE(ul-- == i);
			ASSERT_TRUE(a-- == i);
		}
	}
}

/*
 TEST(CNumZ, Equals) {
 Z a(10L);
 ASSERT_TRUE(a == 10);
 ASSERT_TRUE(a == 10L);

 ASSERT_FALSE(a == 11);
 ASSERT_FALSE(a == 9L);

 ASSERT_TRUE(a == Z(10));
 ASSERT_FALSE(a == Z(9));

 ASSERT_TRUE(Z(0) == Z(0));
 ASSERT_TRUE(Z(0) == -Z(0));
 ASSERT_TRUE(Z(0) == -0);
 ASSERT_TRUE(Z(-0) == -Z(0));
 ASSERT_TRUE(Z(-0) == 0);
 }

 TEST(CNumZ, NotEquals) {
 Z a(1);
 ASSERT_FALSE(a != 1);
 ASSERT_FALSE(a != 1L);

 ASSERT_TRUE(a != 0);
 ASSERT_TRUE(a != -1L);

 ASSERT_TRUE(a != Z(2));
 ASSERT_FALSE(a != Z(1L));
 }

 TEST(CNumZ, LessThan) {
 Z a = 1;
 ASSERT_TRUE(a < 2);
 ASSERT_FALSE(a < 1L);
 ASSERT_FALSE(a < 0);
 ASSERT_FALSE(a < Z(-1));

 a = -2;
 ASSERT_FALSE(a < -3);
 ASSERT_FALSE(a < -2L);
 ASSERT_TRUE(a < -1);
 ASSERT_TRUE(a < Z(0));

 ASSERT_FALSE(Z(0) < -Z(0));
 }

 TEST(CNumZ, GreaterThan) {
 Z a = 1;
 ASSERT_TRUE(a > -1);
 ASSERT_TRUE(a > 0L);
 ASSERT_FALSE(a > 1);
 ASSERT_FALSE(a > Z(2L));

 a = -2;
 ASSERT_TRUE(a > -3);
 ASSERT_FALSE(a > -2L);
 ASSERT_FALSE(a > -1);
 ASSERT_FALSE(a > Z(0));
 }

 TEST(CNumZ, LessThanEqual) {
 Z a = 1;
 ASSERT_TRUE(a <= 2);
 ASSERT_TRUE(a <= 1L);
 ASSERT_FALSE(a <= 0);
 ASSERT_FALSE(a <= Z(-1));

 a = -2;
 ASSERT_FALSE(a <= -3);
 ASSERT_TRUE(a <= -2L);
 ASSERT_TRUE(a <= -1);
 ASSERT_TRUE(a <= Z(0));

 ASSERT_TRUE(Z(0) <= Z(0));
 }

 TEST(CNumZ, GreaterThanEqual) {
 Z a = 1;
 ASSERT_TRUE(a >= -1);
 ASSERT_TRUE(a >= 0L);
 ASSERT_TRUE(a >= 1);
 ASSERT_FALSE(a >= Z(2L));

 a = -2;
 ASSERT_TRUE(a >= -3);
 ASSERT_TRUE(a >= -2L);
 ASSERT_FALSE(a >= -1);
 ASSERT_FALSE(a >= Z(0));

 ASSERT_TRUE(Z(0) <= Z(0));
 }
 */
