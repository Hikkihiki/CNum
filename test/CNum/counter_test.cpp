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
        std::make_tuple(CNum::UNIT_MAX, CNum::UNIT_BIT_SIZE - 1, 0,
                        CNum::UNIT_MAX << (CNum::UNIT_BIT_SIZE - 1),
                        CNum::UNIT_BIT_SIZE - 1, CNum::UNIT_MAX >> 1),
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

class RightShiftTest : public ::testing::TestWithParam<
                           std::tuple<Unit, Unit, Unit, Unit, Unit, Unit>> {};

INSTANTIATE_TEST_CASE_P(
    CNumUnit, RightShiftTest,
    ::testing::Values(
        std::make_tuple(0, 0, 0, 0, 0, 0),
        std::make_tuple(1, 1, 0, 0, 1, 1ULL << (CNum::UNIT_BIT_SIZE - 1)),
        std::make_tuple(3, 2, 0, 0, 2, 3ULL << (CNum::UNIT_BIT_SIZE - 2)),
        std::make_tuple(0xFF, 4, 0, 0xF, 4, 0xFULL
                                                << (CNum::UNIT_BIT_SIZE - 4)),
        std::make_tuple(0xFF, 7, 1, 1, 7, 0x7FULL << (CNum::UNIT_BIT_SIZE - 7)),
        std::make_tuple(0xFAULL << (CNum::UNIT_BIT_SIZE - 8), 3, 0,
                        0x1f4ULL << (CNum::UNIT_BIT_SIZE - 12), 3, 0),
        std::make_tuple(CNum::UNIT_MAX, 1, 1ULL << (CNum::UNIT_BIT_SIZE - 1),
                        CNum::UNIT_MAX, 1, 1ULL << (CNum::UNIT_BIT_SIZE - 1)),
        std::make_tuple(CNum::UNIT_MAX, CNum::UNIT_BIT_SIZE - 1, 0, 1,
                        CNum::UNIT_BIT_SIZE - 1, CNum::UNIT_MAX << 1),
        std::make_tuple(CNum::UNIT_MAX, 15,
                        0x7FFFULL << (CNum::UNIT_BIT_SIZE - 15), CNum::UNIT_MAX,
                        15, 0x7FFFULL << (CNum::UNIT_BIT_SIZE - 15)),
        std::make_tuple(CNum::UNIT_MAX / 2, CNum::UNIT_MAX / 2, 1,
                        CNum::UNIT_MAX, CNum::UNIT_MAX / 2, 0),
        std::make_tuple(0xABCD, 12, 0x1ULL << (CNum::UNIT_BIT_SIZE - 4),
                        0x1ULL << (CNum::UNIT_BIT_SIZE - 4) | 0xA, 12,
                        0xBCDULL << (CNum::UNIT_BIT_SIZE - 12))));

TEST_P(RightShiftTest, SanityTest) {
  Unit unit, shift, filler, exp_unit, exp_shift, exp_filler;
  std::tie(unit, shift, filler, exp_unit, exp_shift, exp_filler) = GetParam();
  if (shift >= sizeof(Unit) * 8) {
    ASSERT_DEATH(CNum::right_shift(unit, shift, filler), "");
  } else {
    CNum::right_shift(unit, shift, filler);
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
        std::make_tuple(0x8ac7230489e80000ULL, 10, 0, 0x6bc75e2d63100000ULL, 10,
                        0x5ULL),
        std::make_tuple(0, 0, 0xFFFF, 0xFFFF, 0, 0),
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
  {
    Counter a("0x56bc75e2d63100000");
    ASSERT_EQ(0x6bc75e2d63100000, a.ull());
  }
}

TEST(CNumCounter, DecStringConstructor) {
  {
    Counter a("");
    ASSERT_EQ(0, a);
  }
  {
    Counter a("0");
    ASSERT_EQ(0, a);
  }
  {
    Counter a("1");
    ASSERT_EQ(1, a);
  }
  {
    Counter a("1234");
    ASSERT_EQ(1234, a);
  }
  {
    Counter a("00123");
    ASSERT_EQ(123, a);
  }
  {
    Counter a("1122334455667788");
    ASSERT_EQ(1122334455667788ULL, a.ull());
  }
  {
    // 16^64
    Counter a("1157920892373161954235709850086879078532699846656405640394575840"
              "07913129639936");
    ASSERT_EQ(0ULL, a.ull());
  }
  {
    // 16^64 + 1234
    Counter a("1157920892373161954235709850086879078532699846656405640394575840"
              "07913129641170");
    ASSERT_EQ(1234ULL, a.ull());
  }
  {
    // 10^19
    Counter a("10000000000000000000");
    ASSERT_EQ(Counter("0x8ac7230489e80000"), a);
    ASSERT_EQ(0x8ac7230489e80000ULL, a.ull());
    ASSERT_EQ(10000000000000000000ULL, a.ull());
    ASSERT_EQ(Counter("0x56bc75e2d63100000"), a * 10);
    ASSERT_EQ(Counter("100000000000000000000"), a * 10);
  }
  {
    // 10^20
    Counter a("100000000000000000000");
    ASSERT_EQ(Counter("0x56bc75e2d63100000"), a);
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
  {
    Counter a("");
    Counter b("");
    ASSERT_EQ(Counter(""), a += b);
  }
  {
    Counter a("8057398573985732095837259083279853798537295837250987923753925793"
              "7459038753985732895");
    Counter b("1");
    ASSERT_EQ(
        Counter(
            "8057398573985732095837259083279853798537295837250987923753925793"
            "7459038753985732896"),
        a += b);
  }
  {

    Counter a("8057398573985732095837259083279853798537295837250987923753925793"
              "7459038753985732895");
    Counter b("1611479714797146419167451816655970759707459167450197584750785158"
              "74918077507971465790");
    ASSERT_EQ(b, a += a);
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
    Counter a("84937593802573428905739573988943275238057305957");
    ASSERT_EQ(0, a -= a);
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

TEST(CNumCounter, MultiplicationAssignment) {
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
  {
    Counter a("123456789012345678901234567890");
    ASSERT_EQ(
        Counter("15241578753238836750495351562536198787501905199875019052100"),
        a *= a);
  }
  {
    Counter a = 1;
    std::string str("1");
    for (int i = 1; i <= 500; i += 10) {
      for (int j = 0; j < 10; j++) {
        str.push_back('0');
      }
      ASSERT_EQ(Counter(str), a *= 10000000000) << i;
    }
  }
}

TEST(CNumCounter, DivisionAssignment) {
  {
    Counter a = 0;
    ASSERT_DEATH(a /= 0, "");
  }
  {
    Counter a = 1;
    ASSERT_DEATH(a /= 0, "");
  }
  {
    Counter a = 0;
    ASSERT_EQ(0, a /= 1);
  }
  {
    Counter a = 1;
    ASSERT_EQ(1, a /= 1);
  }
  {
    Counter a = 2;
    ASSERT_EQ(2, a /= 1);
  }
  {
    Counter a = 2;
    ASSERT_EQ(1, a /= 2);
  }
  {
    Counter a = 3;
    ASSERT_EQ(3, a /= 1);
  }
  {
    Counter a = 3;
    ASSERT_EQ(1, a /= 2);
  }
  {
    Counter a = 0xABCDULL * 0x1234ULL;
    ASSERT_EQ(0xABCDULL, a /= 0x1234ULL);
  }
  {
    Counter a("123456789012345678901234567890");
    ASSERT_EQ(1, a /= a);
  }
  {
    Counter a("1089471298729846791285729571829");
    Counter b("410872198572985729852795843");
    Counter c("447633467791298266595209587598769239788656232418941106847");
    ASSERT_EQ(b, c /= a);
    c = Counter("447633467791298266595209587598769239788656232418941106847");
    ASSERT_EQ(a, c /= b);
  }
  {
    // slow...
    std::string str("1");
    for (int i = 1; i <= 200; i += 10) {
      for (int j = 0; j < 10; j++) {
        str.push_back('0');
      }
    }
    Counter a(str);
    for (int i = 1; i <= 200; i += 10) {
      for (int j = 0; j < 10; j++) {
        str.pop_back();
      }
      ASSERT_EQ(Counter(str), a /= 10000000000) << i;
    }
  }
}

TEST(CNumCounter, ModAssignment) {
  {
    Counter a = 0;
    ASSERT_DEATH(a %= 0, "");
  }
  {
    Counter a = 1;
    ASSERT_DEATH(a %= 0, "");
  }
  {
    Counter a = 0;
    ASSERT_EQ(0, a %= 1);
  }
  {
    Counter a = 1;
    ASSERT_EQ(0, a %= 1);
  }
  {
    Counter a = 2;
    ASSERT_EQ(0, a %= 1);
  }
  {
    Counter a = 2;
    ASSERT_EQ(0, a %= 2);
  }
  {
    Counter a = 3;
    ASSERT_EQ(0, a %= 1);
  }
  {
    Counter a = 3;
    ASSERT_EQ(1, a %= 2);
  }
  {
    Counter a = 0xABCDULL * 0x1234ULL;
    ASSERT_EQ(0, a %= 0x1234ULL);
  }
  {
    Counter a("123456789012345678901234567890");
    ASSERT_EQ(0, a %= a);
  }
  {
    Counter a("1089471298729846791285729571829");
    Counter b("410872198572985729852795843");
    Counter c("447633467791298266595209587598769239788656232418941106847");
    ASSERT_EQ(7, (c + 7) %= a);
    c = Counter("447633467791298266595209587598769239788656232418941106847");
    ASSERT_EQ(13, (c + 13) %= b);
  }
}

TEST(CNumCounter, LeftShiftAssignment) {
  {
    Counter a = 0;
    ASSERT_EQ(0, a <<= 0);
    ASSERT_EQ(0, a <<= 1);
    ASSERT_EQ(0, a <<= 2);
    ASSERT_EQ(0, a <<= 7);
    ASSERT_EQ(0, a <<= 23);
    ASSERT_EQ(0, a <<= 63);
    ASSERT_EQ(0, a <<= 64);
    ASSERT_EQ(0, a <<= 65);
    ASSERT_EQ(0, a <<= CNum::UNIT_MAX);
  }
  {
    Counter a = 1;
    ASSERT_EQ(1, (a = 1, a <<= 0));
    ASSERT_EQ(2, (a = 1, a <<= 1));
    ASSERT_EQ(4, (a = 1, a <<= 2));
    ASSERT_EQ(128, (a = 1, a <<= 7));
    ASSERT_EQ(8388608, (a = 1, a <<= 23));
    ASSERT_EQ(9223372036854775808ULL, (a = 1, a <<= 63));
    ASSERT_EQ(Counter("18446744073709551616"), (a = 1, a <<= 64));
    ASSERT_EQ(Counter("36893488147419103232"), (a = 1, a <<= 65));
  }
  {
    std::string str(
        "0x0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABC"
        "DEF0123456789ABCDEF");
    Counter a(str);
    for (int i = 0; i < 1244 / 4; i++) {
      str.push_back('0');
    }
    ASSERT_EQ(Counter(str), a <<= 1244);
  }
  {
    std::string str(
        "0x0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABC"
        "DEF0123456789ABCDEF");
    Counter a(str);
    for (int i = 0; i < 2048 / 4; i++) {
      str.push_back('0');
    }
    ASSERT_EQ(Counter(str), a <<= 2048);
  }
}

TEST(CNumCounter, RightShiftAssignment) {
  {
    Counter a = 0;
    ASSERT_EQ(0, a >>= 0);
    ASSERT_EQ(0, a >>= 1);
    ASSERT_EQ(0, a >>= 2);
    ASSERT_EQ(0, a >>= 7);
    ASSERT_EQ(0, a >>= 23);
    ASSERT_EQ(0, a >>= 63);
    ASSERT_EQ(0, a >>= 64);
    ASSERT_EQ(0, a >>= 65);
    ASSERT_EQ(0, a >>= CNum::UNIT_MAX);
  }
  {
    Counter a = 1;
    ASSERT_EQ(1, (a = 1, a >>= 0));
    ASSERT_EQ(1, (a = 2, a >>= 1));
    ASSERT_EQ(1, (a = 4, a >>= 2));
    ASSERT_EQ(1, (a = 6, a >>= 2));
    ASSERT_EQ(2, (a = 128, a >>= 6));
    ASSERT_EQ(1, (a = 128, a >>= 7));
    ASSERT_EQ(1, (a = 8388608, a >>= 23));
    ASSERT_EQ(1, (a = 9223372036854775808ULL, a >>= 63));
    ASSERT_EQ(1, Counter("18446744073709551616") >>= 64);
    ASSERT_EQ(1, Counter("36893488147419103232") >>= 65);
  }
  {
    std::string str(
        "0x0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123"
        "456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789"
        "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF012345"
        "6789ABCDEF0123456789ABCDEF0123456789ABCDEF");
    Counter a(str);
    for (int i = 0; i < 444 / 4; i++) {
      str.pop_back();
    }
    ASSERT_EQ(Counter(str), a >>= 444);
  }
  {
    std::string str(
        "0x0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123"
        "456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789"
        "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF012345"
        "456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789"
        "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF012345"
        "456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789"
        "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF012345"
        "6789ABCDEF0123456789ABCDEF0123456789ABCDEF");
    Counter a(str);
    for (int i = 0; i < 1024 / 4; i++) {
      str.pop_back();
    }
    ASSERT_EQ(Counter(str), a >>= 1024);
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
  {
    Counter a = 3;
    ASSERT_EQ(6, a * 2);
    ASSERT_EQ(0, a * 0);
    ASSERT_EQ(987654321ULL * 3, a = a * 987654321ULL);

    Counter b = 1234567890ULL;
    ASSERT_EQ(987654321ULL * 1234567890ULL * 3, a * b);
  }
  {
    ASSERT_EQ(Counter(""), Counter("") * Counter(""));
    ASSERT_EQ(Counter("44638112098138407571318574082261234507924739492194936899"
                      "01791797045200"),
              Counter("129034829048290428904280494390583490") *
                  Counter("34593847589345734085430857340853480"));
  }
}

TEST(CNumCounter, Division) {
  {
    Counter a = 3;
    ASSERT_EQ(1, a / 2);
    ASSERT_DEATH(a / 0, "");
    ASSERT_EQ(111111, a = 333333 / a);

    Counter b = 1234567899ULL;
    ASSERT_EQ(123456789ULL, b / 10);
  }
  {
    ASSERT_EQ(Counter("129034829048290428904280494390583490"),
              Counter("44638112098138407571318574082261234507924739492194936899"
                      "01791797045200") /
                  Counter("34593847589345734085430857340853480"));
  }
}

TEST(CNumCounter, Mod) {
  {
    Counter a = 3;
    ASSERT_EQ(1, a % 2);
    ASSERT_DEATH(a % 0, "");
    ASSERT_EQ(2, a = 333335 % a);

    Counter b = 1234567894ULL;
    ASSERT_EQ(4, b % 10);
  }
  {
    ASSERT_EQ(
        Counter("3069812820931554"),
        Counter("447633467791298266595209587598769239788656232418941106847") %
            Counter("43985734954932857"));
  }
}

TEST(CNumCounter, LeftShift) {
  {
    Counter a = 0;
    ASSERT_EQ(0, a << 0);
    ASSERT_EQ(0, a << 1);
    ASSERT_EQ(987654321ULL * 16, Counter(987654321ULL) << 4);
  }
  {
    ASSERT_EQ(Counter(""), Counter("") << Counter(""));
    Counter a("4463811209813840757131857408226123450792473949219493689901791797"
              "045200");
    ASSERT_EQ(a * Counter("1267650600228229401496703205376"), a << 100);
  }
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

TEST(CNumCounter, BitSize) {
  ASSERT_EQ(0, Counter("0").bitSize());
  ASSERT_EQ(1, Counter("1").bitSize());
  ASSERT_EQ(2, Counter("2").bitSize());
  ASSERT_EQ(2, Counter("3").bitSize());
  ASSERT_EQ(3, Counter("4").bitSize());
  ASSERT_EQ(CNum::UNIT_BIT_SIZE, Counter(CNum::UNIT_MAX).bitSize());
  ASSERT_EQ(20 * 4, Counter("0xFFFFFFFFFFFFFFFFFFFF").bitSize());
  ASSERT_EQ(20 * 4 - 3, Counter("0x1FFFFFFFFFFFFFFFFFFF").bitSize());
}

TEST(CNumCounter, HexConversion) {
  {
    Counter a("0x0123456789abcdef");
    ASSERT_EQ(std::string("0x123456789ABCDEF"), a.hex());
  }
  {
    ASSERT_EQ(std::string("0x0"), Counter("0x0").hex());
    ASSERT_EQ(std::string("0x1"), Counter("0x1").hex());
    ASSERT_EQ(std::string("0x13814573985739172498537957190857139857398457398573"
                          "198573198ABCDEF"),
              Counter("0x138145739857391724985379571908571398573984573985731985"
                      "73198abcdef")
                  .hex());
  }
}

TEST(CNumCounter, Pow) {
  ASSERT_DEATH(Counter(0).pow(0), "");
  ASSERT_EQ(1, Counter(1).pow(0));
  ASSERT_EQ(1,
            Counter("2037528397598437523890572397593827539205703298").pow(0));
  ASSERT_EQ(1, Counter(1).pow(Counter("3120498204280427539857395739379580492742"
                                      "89472894274982472908421790")));
  ASSERT_EQ(0, Counter(0).pow(1));
  ASSERT_EQ(Counter("43057239085732957390573295372598327593250327"),
            Counter("43057239085732957390573295372598327593250327").pow(1));

  ASSERT_EQ(Counter("1853925837685969880435601434684976711967790086793331727853"
                    "791677722018016751808485606929"),
            Counter("43057239085732957390573295372598327593250327").pow(2));
  ASSERT_EQ(
      Counter("48519278097689642681155855396759336072749841943521979872827"),
      Counter(3).pow(123));
  ASSERT_EQ(Counter("141050039560662968926103"), Counter(23).pow(17));
  ASSERT_EQ(Counter("489801110321660601428677553"), Counter(113).pow(13));
}

TEST(CNumCounter, Log2) {
  ASSERT_DEATH(Counter("0").log2(), "");
  ASSERT_EQ(0, Counter("1").log2());
  ASSERT_EQ(1, Counter("2").log2());
  ASSERT_EQ(1, Counter("3").log2());
  ASSERT_EQ(2, Counter("4").log2());
  ASSERT_EQ(117, Counter("189234729847298573987298673896981073").log2());
  ASSERT_EQ(
      207,
      Counter("0xABCDEF4385927358932759345872389537593207532089ABCDEF").log2());
}

TEST(CNumCounter, isSet) {
  Counter a("0x11223344556677889900AABBCCDDEEFF");
  ASSERT_TRUE(a.isSet(0));
  ASSERT_TRUE(a.isSet(1));
  ASSERT_TRUE(a.isSet(2));
  ASSERT_FALSE(a.isSet(8));

  ASSERT_FALSE(a.isSet(127));
  ASSERT_FALSE(a.isSet(126));
  ASSERT_FALSE(a.isSet(125));
  ASSERT_TRUE(a.isSet(124));

  ASSERT_FALSE(a.isSet(123));
  ASSERT_FALSE(a.isSet(122));
  ASSERT_FALSE(a.isSet(121));
  ASSERT_TRUE(a.isSet(120));

  a = 0;
  ASSERT_FALSE(a.isSet(0));
  ASSERT_FALSE(a.isSet(1));
  ASSERT_FALSE(a.isSet(2));
}

TEST(CNumCounter, sqrt) {
  ASSERT_EQ(0, sqrt(Counter(0)));
  ASSERT_EQ(1, sqrt(Counter(1)));
  ASSERT_EQ(1, sqrt(Counter(2)));
  ASSERT_EQ(1, sqrt(Counter(3)));
  ASSERT_EQ(2, sqrt(Counter(4)));
  ASSERT_EQ(2, sqrt(Counter(5)));
  ASSERT_EQ(2, sqrt(Counter(6)));
  ASSERT_EQ(2, sqrt(Counter(7)));
  ASSERT_EQ(2, sqrt(Counter(8)));
  ASSERT_EQ(3, sqrt(Counter(9)));
  ASSERT_EQ(3, sqrt(Counter(10)));
  ASSERT_EQ(3, sqrt(Counter(11)));
  Counter c("420857389572398");
  ASSERT_EQ(c - 1, sqrt(c * c - 1));
  ASSERT_EQ(c, sqrt(c * c));
  ASSERT_EQ(c, sqrt(c * c + 1));
}

TEST(CNumCounter, gcd) {
  ASSERT_DEATH(gcd(Counter(0), Counter(0)), "");
  ASSERT_DEATH(gcd(Counter(0), Counter(1)), "");
  ASSERT_DEATH(gcd(Counter(1), Counter(0)), "");
  ASSERT_EQ(1, gcd(Counter(1), Counter(1)));
  ASSERT_EQ(1, gcd(Counter(1), Counter(2)));
  ASSERT_EQ(1, gcd(Counter(1), Counter(10)));
  ASSERT_EQ(1, gcd(Counter(1), Counter("34809257398573985732957398537493")));
  ASSERT_EQ(
      1, gcd(Counter("423058734895724985734985473985724389057"), Counter(1)));
  ASSERT_EQ(2, gcd(Counter(2), Counter(2)));
  ASSERT_EQ(1, gcd(Counter(2), Counter(3)));
  ASSERT_EQ(2, gcd(Counter(2), Counter(4)));
  ASSERT_EQ(3, gcd(Counter(3), Counter(6)));
  ASSERT_EQ(4, gcd(Counter(8), Counter(4)));
  ASSERT_EQ(4, gcd(Counter(8), Counter(4)));
  ASSERT_EQ(2, gcd(Counter(8), Counter(6)));

  Counter a("2439507234584375982375938457239537249");
  Counter b("483975239857923485743985723985");
  Counter c("1180661098993112981928553528130962217532052"
            "961002553849763640217265");
  ASSERT_EQ(a, gcd(a, c));
  ASSERT_EQ(a, gcd(c, a));
  ASSERT_EQ(b, gcd(b, c));
  ASSERT_EQ(b, gcd(c, b));
}

TEST(CNumCounter, lcm) {
  ASSERT_DEATH(lcm(Counter(0), Counter(0)), "");
  ASSERT_DEATH(lcm(Counter(0), Counter(1)), "");
  ASSERT_DEATH(lcm(Counter(1), Counter(0)), "");
  ASSERT_EQ(1, lcm(Counter(1), Counter(1)));
  ASSERT_EQ(2, lcm(Counter(1), Counter(2)));
  ASSERT_EQ(10, lcm(Counter(1), Counter(10)));
  ASSERT_EQ(Counter("34809257398573985732957398537493"),
            lcm(Counter(1), Counter("34809257398573985732957398537493")));
  ASSERT_EQ(
      Counter("423058734895724985734985473985724389057"),
      lcm(Counter("423058734895724985734985473985724389057"), Counter(1)));
  ASSERT_EQ(2, lcm(Counter(2), Counter(2)));
  ASSERT_EQ(6, lcm(Counter(2), Counter(3)));
  ASSERT_EQ(4, lcm(Counter(2), Counter(4)));
  ASSERT_EQ(6, lcm(Counter(3), Counter(6)));
  ASSERT_EQ(8, lcm(Counter(8), Counter(4)));
  ASSERT_EQ(8, lcm(Counter(8), Counter(4)));
  ASSERT_EQ(24, lcm(Counter(8), Counter(6)));

  Counter a("2439507234584375982375938457239537249");
  Counter b("483975239857923485743985723985");
  Counter c("1180661098993112981928553528130962217532052"
            "961002553849763640217265");
  ASSERT_EQ(c, lcm(a, c));
  ASSERT_EQ(c, lcm(c, a));
  ASSERT_EQ(c, lcm(b, c));
  ASSERT_EQ(c, lcm(c, b));
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
