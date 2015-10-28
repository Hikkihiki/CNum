#include "../../include/counter.hpp"

#include <tuple>
#include <vector>
#include "gtest/gtest.h"
using ::testing::Test;
using ::CNum::Counter;
using ::CNum::Unit;

class AddTest : public ::testing::TestWithParam<
                    std::tuple<Unit, Unit, Unit, Unit, Unit, Unit>> {};
INSTANTIATE_TEST_CASE_P(
    CNumUnit, AddTest,
    ::testing::Values(std::make_tuple(0, 0, 0, 0, 0, 0),
                      std::make_tuple(0, 1, 0, 1, 1, 0),
                      std::make_tuple(2, 1, 0, 3, 1, 0),
                      std::make_tuple(CNum::UNIT_MAX, 1, 0, 0, 1, 1),
                      std::make_tuple(CNum::UNIT_MAX, 1, 1, 1, 1, 1),
                      std::make_tuple(CNum::UNIT_MAX, CNum::UNIT_MAX, 1,
                                      CNum::UNIT_MAX, CNum::UNIT_MAX, 1),
                      std::make_tuple(CNum::UNIT_MAX / 2, CNum::UNIT_MAX / 2, 1,
                                      CNum::UNIT_MAX, CNum::UNIT_MAX / 2, 0)));
TEST_P(AddTest, SanityTest) {
  Unit a, b, carry, exp_a, exp_b, exp_carry;
  std::tie(a, b, carry, exp_a, exp_b, exp_carry) = GetParam();
  CNum::add(a, b, carry);
  ASSERT_EQ(exp_a, a);
  ASSERT_EQ(exp_b, b);
  ASSERT_EQ(exp_carry, carry);
};

class SubtractTest : public ::testing::TestWithParam<
                         std::tuple<Unit, Unit, Unit, Unit, Unit, Unit>> {};
INSTANTIATE_TEST_CASE_P(
    CNumUnit, SubtractTest,
    ::testing::Values(std::make_tuple(0, 0, 0, 0, 0, 0),
                      std::make_tuple(2, 1, 0, 1, 1, 0),
                      std::make_tuple(3, 4, 0, CNum::UNIT_MAX, 4, 1),
                      std::make_tuple(0, 0, 1, CNum::UNIT_MAX, 0, 1),
                      std::make_tuple(CNum::UNIT_MAX, CNum::UNIT_MAX, 0, 0,
                                      CNum::UNIT_MAX, 0),
                      std::make_tuple(CNum::UNIT_MAX, CNum::UNIT_MAX, 1,
                                      CNum::UNIT_MAX, CNum::UNIT_MAX, 1),
                      std::make_tuple(CNum::UNIT_MAX, CNum::UNIT_MAX / 2, 1,
                                      CNum::UNIT_MAX - CNum::UNIT_MAX / 2 - 1,
                                      CNum::UNIT_MAX / 2, 0),
                      std::make_tuple(CNum::UNIT_MAX / 2, CNum::UNIT_MAX / 2, 1,
                                      CNum::UNIT_MAX, CNum::UNIT_MAX / 2, 1)));
TEST_P(SubtractTest, SanityTest) {
  Unit a, b, carry, exp_a, exp_b, exp_carry;
  std::tie(a, b, carry, exp_a, exp_b, exp_carry) = GetParam();
  CNum::sub(a, b, carry);
  ASSERT_EQ(exp_a, a);
  ASSERT_EQ(exp_b, b);
  ASSERT_EQ(exp_carry, carry);
};

class LeftShiftTest : public ::testing::TestWithParam<
                          std::tuple<Unit, Unit, Unit, Unit, Unit, Unit>> {};
INSTANTIATE_TEST_CASE_P(
    CNumUnit, LeftShiftTest,
    ::testing::Values(
        std::make_tuple(0, 0, 0, 0, 0, 0), std::make_tuple(1, 1, 0, 2, 1, 0),
        std::make_tuple(3, 2, 0, 12, 2, 0),
        std::make_tuple(0x0F, 4, 0, 0xF0, 4, 0),
        std::make_tuple(0x7F, 7, 1, 0x3F81, 7, 0),
        std::make_tuple(0xFA00000000000000, 3, 1, 0xD000000000000001, 3, 7),
        std::make_tuple(CNum::UNIT_MAX, 1, 1, CNum::UNIT_MAX, 1, 1),
        std::make_tuple(CNum::UNIT_MAX, sizeof(Unit) * 8 - 1, 0,
                        CNum::UNIT_MAX << (sizeof(Unit) * 8 - 1),
                        sizeof(Unit) * 8 - 1, CNum::UNIT_MAX >> 1),
        std::make_tuple(CNum::UNIT_MAX, 15, 0x7FFF, CNum::UNIT_MAX, 15, 0x7FFF),
        std::make_tuple(CNum::UNIT_MAX / 2, CNum::UNIT_MAX / 2, 1,
                        CNum::UNIT_MAX, CNum::UNIT_MAX / 2, 0),
        std::make_tuple(0xABCD, 12, 1, 0xABCD001, 12, 0)));
TEST_P(LeftShiftTest, SanityTest) {
  Unit unit, shift, filler, exp_unit, exp_shift, exp_filler;
  std::tie(unit, shift, filler, exp_unit, exp_shift, exp_filler) = GetParam();
  if (shift >= sizeof(Unit) * 8) {
    ASSERT_DEATH(CNum::left_shift(unit, shift, filler), "");
  } else {
    CNum::left_shift(unit, shift, filler);
    ASSERT_EQ(exp_unit, unit);
    ASSERT_EQ(exp_shift, shift);
    ASSERT_EQ(exp_filler, filler);
  }
};

class MultiplicationTest : public ::testing::TestWithParam<
                               std::tuple<Unit, Unit, Unit, Unit, Unit, Unit>> {
};
INSTANTIATE_TEST_CASE_P(
    CNumUnit, MultiplicationTest,
    ::testing::Values(
        std::make_tuple(0, 0, 0, 0, 0, 0), std::make_tuple(2, 1, 0, 2, 1, 0),
        std::make_tuple(3, 4, 1, 13, 4, 0),
        std::make_tuple(0xAABBCCDD, 0x11223344, 0,
                        0xAABBCCDDULL * 0x11223344ULL, 0x11223344, 0),
        std::make_tuple(0xFFFFFFFF, 0x100000000, 0, 0xFFFFFFFF00000000ULL,
                        0x100000000, 0),
        std::make_tuple(CNum::UNIT_MAX, CNum::UNIT_MAX, 0, 1, CNum::UNIT_MAX,
                        CNum::UNIT_MAX - 1),
        std::make_tuple(CNum::UNIT_MAX, CNum::UNIT_MAX, CNum::UNIT_MAX, 0,
                        CNum::UNIT_MAX, CNum::UNIT_MAX),
        std::make_tuple(CNum::UNIT_MAX, 0, CNum::UNIT_MAX, CNum::UNIT_MAX, 0,
                        0)));
TEST_P(MultiplicationTest, SanityTest) {
  Unit a, b, carry, exp_a, exp_b, exp_carry;
  std::tie(a, b, carry, exp_a, exp_b, exp_carry) = GetParam();
  CNum::mul(a, b, carry);
  ASSERT_EQ(exp_a, a);
  ASSERT_EQ(exp_b, b);
  ASSERT_EQ(exp_carry, carry);
};

/*
TEST_P(CNumUnit, LeftShift) {
  Params param = GetParam();
  Unit a = param.a;
  Unit b = param.b;
  Unit carry = param.carry;
  CNum::add(a, b, carry);
  ASSERT_EQ(param.e_a, a);
  ASSERT_EQ(param.e_b, b);
  ASSERT_EQ(param.e_carry, carry);
}
*/
/*
 TEST(CNumUnit, Add) {
 {
 Unit a, b, carry;
 a = b = carry = 0;
 CNum::add(a, b, carry);
 ASSERT_EQ(0, a);
 ASSERT_EQ(0, b);
 ASSERT_EQ(0, carry);
 }
 {
 Unit a, b, carry;
 a = 0;
 b = 1;
 carry = 0;
 CNum::add(a, b, carry);
 ASSERT_EQ(0, 1);
 ASSERT_EQ(0, 1);
 ASSERT_EQ(0, carry);
 }
 {
 Unit carry = 0;
 ASSERT_EQ(3, CNum::add(2, 1, carry, carry));
 ASSERT_EQ(0, carry);
 }
 {
 Unit carry = 0;
 ASSERT_EQ(CNum::UNIT_MAX, CNum::add(CNum::UNIT_MAX, 0, carry, carry));
 ASSERT_EQ(0, carry);
 }
 {
 Unit carry = 0;
 ASSERT_EQ(0, CNum::add(CNum::UNIT_MAX, 1, carry, carry));
 ASSERT_EQ(1, carry);
 }
 {
 Unit carry = 0;
 ASSERT_EQ(2, CNum::add(3, CNum::UNIT_MAX, carry, carry));
 ASSERT_EQ(1, carry);
 }
 {
 Unit carry = 0;
 ASSERT_EQ(CNum::UNIT_MAX << 1,
 CNum::add(CNum::UNIT_MAX, CNum::UNIT_MAX, carry, carry));
 ASSERT_EQ(1, carry);
 }
 {
 Unit carry = 0;
 ASSERT_EQ(CNum::UNIT_MAX - 1,
 CNum::add(CNum::UNIT_MAX / 2, CNum::UNIT_MAX / 2, carry,
 carry));
 ASSERT_EQ(0, carry);
 }
 }
 */

TEST(CNumCounter, DefaultConstructor) {
  {
    Counter a;
    ASSERT_EQ(0, a);
  }
}

TEST(CNumCounter, ExpicitConstructor) {
  {
    Counter a(1);
    ASSERT_EQ(1, a);
  }
  {
    Counter a(0);
    ASSERT_EQ(0, a);
  }
  {
    Counter a(-1);
    ASSERT_EQ(-1, a);
  }
  {
    Counter a(192837465L);
    ASSERT_EQ(192837465L, a);
  }
}

TEST(CNumCounter, HexStringConstructor) {
  {
    Counter a("0x");
    ASSERT_EQ(0, a);
  }
  {
    Counter a("0x0");
    ASSERT_EQ(0, a);
  }
  {
    Counter a("0x1");
    ASSERT_EQ(1, a);
  }
  {
    Counter a("0xABCD");
    ASSERT_EQ(0xABCD, a);
  }
  {
    Counter a("0x0123");
    ASSERT_EQ(0x123, a);
  }
  {
    Counter a("0x11223344556677881122334455667788");
    ASSERT_EQ(0x1122334455667788, a.ull());
  }
}

TEST(CNumCounter, CopyConstructor) {
  {
    Counter a = Counter(10);
    ASSERT_EQ(10, a);
  }
  {
    Counter a = -10;
    ASSERT_EQ(-10, a);
  }
  {
    Counter a = -0;
    ASSERT_EQ(0, a);
  }
}

TEST(CNumCounter, CopyAssignment) {
  Counter a;
  ASSERT_EQ(0, a);
  a = 20;
  ASSERT_EQ(20, a);
  a = 30L;
  ASSERT_EQ(30, a);
  a = -20;
  ASSERT_EQ(-20, a);
}

TEST(CNumCounter, ULLConversion) {
  {
    Counter a(0);
    ASSERT_EQ(0, a.ull());
  }
}

TEST(CNumCounter, PrefixIncrement) {
  Counter a = 0;
  unsigned long ul = 0;
  ASSERT_EQ(1, ++ul);
  ASSERT_EQ(1, ++a);
  ASSERT_EQ(2, ++ul);
  ASSERT_EQ(2, ++a);
  a = 0;
  ul = 0;

  // bottleneck at == comparision;
  for (unsigned long i = 0; i < 999999; i++) {
    ASSERT_EQ(i + 1, ++ul);
    ASSERT_EQ(i + 1, ++a);
  }
}

TEST(CNumCounter, PostfixIncrement) {
  Counter a = 0;
  unsigned long ul = 0;

  ASSERT_EQ(0, ul++);
  ASSERT_EQ(0, a++);
  ASSERT_EQ(1, ul++);
  ASSERT_EQ(1, a++);
  ASSERT_EQ(2, ul);
  ASSERT_EQ(2, a);
  a = 999999;
  ul = 999999;
  // bottleneck at == comparision;
  for (unsigned long i = 999999; i < 2 * 999999; i++) {
    ASSERT_EQ(i, ul++);
    ASSERT_EQ(i, a++);
  }
}

TEST(CNumCounter, AdditionAssignment) {
  {
    Counter a = 0;
    ASSERT_EQ(0, a += 0);
    ASSERT_EQ(13, a += 13);
    ASSERT_EQ(999999999 + 13, a += 999999999);
  }
  {
    Counter a = 1;
    ASSERT_EQ(1, a += 0);
  }
  {
    Counter b = 0;
    Counter a = b;
    for (long i = 0; i < 1000000LL; ++i) {
      ASSERT_EQ(b + i, a += i);
      b = a;
    }
  }
}

TEST(CNumCounter, SubtractionAssignment) {
  {
    Counter a = 0;
    ASSERT_EQ(0, a -= 0);
    ASSERT_DEATH(a -= 1, "");
  }
  {
    Counter a = 1;
    ASSERT_EQ(1, a -= 0);
  }
  {
    Counter a = 99999999ULL;
    ASSERT_EQ(99999999ULL - 1234567, a -= 1234567);
  }
  {
    Counter b = 99999999ULL;
    Counter a = b;
    for (long i = 0; i < 1000LL; ++i) {
      ASSERT_EQ(b - i, a -= i);
      b = a;
    }
  }
}

TEST(CNumCounter, Multiplicationssignment) {
  {
    Counter a = 0;
    ASSERT_EQ(0, a *= 0);
  }
  {
    Counter a = 1;
    ASSERT_EQ(0, a *= 0);
  }
  {
    Counter a = 1;
    ASSERT_EQ(1, a *= 1);
  }
  {
    Counter a = 1;
    ASSERT_EQ(2, a *= 2);
  }
  {
    Counter a = 3;
    ASSERT_EQ(3, a *= 1);
  }
  {
    Counter a = 0xABCDULL;
    ASSERT_EQ(0xABCDULL * 0x1234ULL, a *= 0x1234ULL);
  }
}

TEST(CNumCounter, Addition) {
  Counter a = 0;
  ASSERT_EQ(0, a + 0);
  ASSERT_EQ(1, a + 1);
  ASSERT_EQ(987654321LL, a = a + 987654321LL);

  Counter b = 1234567890LL;
  ASSERT_EQ(987654321LL + 1234567890LL, a + b);
  ASSERT_EQ(3 * 1234567890LL, b + b + b);
}

TEST(CNumCounter, Muplication) {
  Counter a = 3;
  ASSERT_EQ(6, a * 2);
  ASSERT_EQ(0, a * 0);
  ASSERT_EQ(987654321ULL * 3, a = a * 987654321ULL);

  Counter b = 1234567890ULL;
  ASSERT_EQ(987654321ULL * 1234567890ULL * 3, a * b);
}

TEST(CNumCounter, Equals) {
  Counter a(10L);
  ASSERT_TRUE(a == 10);
  ASSERT_TRUE(a == 10L);
  ASSERT_TRUE(10 == a);
  ASSERT_TRUE(10L == a);

  ASSERT_FALSE(a == 11);
  ASSERT_FALSE(a == 9L);
  ASSERT_FALSE(11 == a);
  ASSERT_FALSE(9L == a);

  ASSERT_TRUE(a == Counter(10));
  ASSERT_TRUE(Counter(10) == a);
  ASSERT_FALSE(a == Counter(9));
  ASSERT_FALSE(Counter(9) == a);

  ASSERT_TRUE(a == a);
}

TEST(CNumCounter, NotEquals) {
  Counter a(1);
  ASSERT_FALSE(a != 1);
  ASSERT_FALSE(a != 1L);
  ASSERT_FALSE(1 != a);
  ASSERT_FALSE(1L != a);

  ASSERT_TRUE(a != 0);
  ASSERT_TRUE(a != -1L);
  ASSERT_TRUE(0 != a);
  ASSERT_TRUE(-1L != a);

  ASSERT_TRUE(a != Counter(2));
  ASSERT_FALSE(a != Counter(1L));

  ASSERT_FALSE(a != a);
}

TEST(CNumCounter, LessThan) {
  Counter a = 1;
  ASSERT_TRUE(a < 2);
  ASSERT_FALSE(a < 1L);
  ASSERT_FALSE(a < 0);

  ASSERT_FALSE(2 < a);
  ASSERT_FALSE(1L < a);
  ASSERT_TRUE(0 < a);
}

TEST(CNumCounter, GreaterThan) {
  Counter a = 1;
  ASSERT_FALSE(a > 2);
  ASSERT_FALSE(a > 1L);
  ASSERT_TRUE(a > 0);

  ASSERT_TRUE(2 > a);
  ASSERT_FALSE(1L > a);
  ASSERT_FALSE(0 > a);

  ASSERT_FALSE(a > a);
}

TEST(CNumCounter, LessThanEqual) {
  Counter a = 1;
  ASSERT_TRUE(a <= 2);
  ASSERT_TRUE(a <= 1L);
  ASSERT_FALSE(a <= 0);

  ASSERT_FALSE(2 <= a);
  ASSERT_TRUE(1L <= a);
  ASSERT_TRUE(0 <= a);

  ASSERT_TRUE(a <= a);
}

TEST(CNumCounter, GreaterThanEqual) {
  Counter a = 1;
  ASSERT_TRUE(a >= 0L);
  ASSERT_TRUE(a >= 1);
  ASSERT_FALSE(a >= Counter(2L));

  ASSERT_FALSE(0L >= a);
  ASSERT_TRUE(1 >= a);
  ASSERT_TRUE(Counter(2L) >= a);

  ASSERT_TRUE(a >= a);
}

/*
 TEST(CNumCounter, PrefixDecrement) {
 {
 Counter a = 1;
 unsigned long ul = 1;
 ASSERT_TRUE(--ul == 0);
 ASSERT_TRUE(--a == 0);
 ASSERT_TRUE(--ul == (unsigned long )-1);
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
 */
