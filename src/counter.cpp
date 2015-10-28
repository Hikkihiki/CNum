#include "../include/counter.hpp"

#include <algorithm>
#include <cassert>
#include <sstream>

void CNum::add(Unit &unit1, const Unit &unit2, Unit &carry) {
  carry = (unit1 += carry) < carry;
  carry += (unit1 += unit2) < unit2;
  assert(carry <= 1);
}

void CNum::sub(Unit &unit1, const Unit &unit2, Unit &borrow) {
  Unit u = unit1;
  borrow = (unit1 -= borrow) > u;
  u = unit1;
  borrow += (unit1 -= unit2) > u;
  assert(borrow <= 1);
}

// Note that: 0xFFFF*0xFFFF+0xFFFF = 0xFFFF0000,
// the carry won't overflow in the worst case
void CNum::mul(Unit &unit1, const Unit &unit2, Unit &carry) {
  assert(CNum::UNIT_BIT_SIZE % 2 == 0);
  const Unit HALF_BIT_SIZE = CNum::UNIT_BIT_SIZE / 2;
  const Unit lf = CNum::UNIT_MAX >> HALF_BIT_SIZE;
  const Unit hf = CNum::UNIT_MAX << HALF_BIT_SIZE;
  const Unit lu1 = unit1 & lf;
  const Unit hu1 = (unit1 & hf) >> HALF_BIT_SIZE;
  const Unit lu2 = unit2 & lf;
  const Unit hu2 = (unit2 & hf) >> HALF_BIT_SIZE;

  unit1 = lu1 * lu2;
  const Unit s1 = hu1 * lu2;
  const Unit s2 = lu1 * hu2;
  const Unit s3 = hu1 * hu2;

  add(unit1, 0, carry);

  Unit carry2 = 0;
  add(unit1, (s1 & lf) << HALF_BIT_SIZE, carry2);
  carry += carry2;

  Unit carry3 = 0;
  add(unit1, (s2 & lf) << HALF_BIT_SIZE, carry3);
  carry += carry3;

  carry += ((s1 & hf) >> HALF_BIT_SIZE);
  carry += ((s2 & hf) >> HALF_BIT_SIZE);
  carry += s3;
}

void CNum::left_shift(Unit &unit, const Unit &shift, Unit &filler) {
  assert(0 <= shift && shift < sizeof(Unit) * 8);
  Unit newFiller = unit >> (sizeof(Unit) * 8 - shift);
  unit <<= shift;
  unit |= filler;
  filler = newFiller;
}

CNum::Counter::Counter(unsigned long long v) : value() {
  assert(sizeof(unsigned long long) <= sizeof(Unit));
  value.push_back(v);
  assert(isNormalized());
}

CNum::Counter::Counter(const std::string &s) : value() {
  if (s.size() == 0) {
    setZero();
    return;
  }

  // Parse the correct base
  auto rBegin = s.crbegin();
  auto rEnd = s.crend();
  Unit base = 10;
  if (s.size() >= 2 && s[0] == '0') {
    if (s[1] == 'x') {
      base = 16;
      rEnd -= 2;
    } else if (s[1] == 'b') {
      base = 2;
      rEnd -= 2;
    }
  }

  // rBegin and rEnd is set
  if (base == 16) {
    const Unit CHAR_SIZE = 4;
    // 1 character in hex occupied 4 bits

    Index shift = 0;
    Unit unit = 0;
    for (auto itr = rBegin; itr != rEnd; itr++) {
      Unit c = *itr;
      if ('0' <= c && c <= '9') {
        c -= '0';
      } else if ('a' <= c && c <= 'f') {
        c = c - 'a' + 10;

      } else if ('A' <= c && c <= 'F') {
        c = c - 'A' + 10;
      } else {
        throw;
      }
      unit |= (c << shift);
      shift = (shift + CHAR_SIZE) % UNIT_BIT_SIZE;
      if (shift == 0) {
        value.push_back(unit);
        unit = 0;
      }
    }
    value.push_back(unit);
    normalize();
  } else if (base == 10) {
    // Naive algorithm that make use of multiplication and addition
    *this = 0;
    Counter exp = 1;
    for (auto itr = rBegin; itr != rEnd; itr++, exp *= 10) {
      Unit c = *itr;
      if ('0' <= c && c <= '9') {
        c -= '0';
      } else {
        throw;
      }
      *this += (c * exp);
    }
  } else {
    throw;
  }
  assert(isNormalized());
}

std::ostream &CNum::operator<<(std::ostream &out, const Counter &rhs) {
  for (Index i = rhs.value.size() - 1; i >= 0; --i) {
    out << rhs.value[i];
  }
  return out;
}

CNum::Counter::Counter() : Counter(0) {}

CNum::Counter::Counter(const Counter &c) : Counter(0) { *this = c; }

void CNum::Counter::setZero() {
  value.clear();
  value.push_back(0);
  assert(isNormalized());
}

bool CNum::Counter::isNormalized() const {
  return value.size() > 1 ? (value.back() != 0) : (value.size() == 1);
}

void CNum::Counter::normalize() {
  // trim leading zero
  while (value.size() > 1 && value.back() == 0) {
    value.pop_back();
  }
  assert(isNormalized());
}

bool CNum::operator==(const Counter &lhs, const Counter &rhs) {
  assert(lhs.isNormalized());
  assert(rhs.isNormalized());
  return lhs.value == rhs.value;
}

bool CNum::operator!=(const Counter &lhs, const Counter &rhs) {
  return !(lhs == rhs);
}

bool CNum::operator<(const Counter &lhs, const Counter &rhs) {
  assert(lhs.isNormalized());
  assert(rhs.isNormalized());
  if (lhs.value.size() == rhs.value.size()) {
    for (Index pos = lhs.value.size() - 1; pos >= 0; --pos) {
      if (lhs.value[pos] == rhs.value[pos])
        continue;
      return lhs.value[pos] < rhs.value[pos];
    }
    return false;
  }
  return lhs.value.size() < rhs.value.size();
}

bool CNum::operator>(const Counter &lhs, const Counter &rhs) {
  return !((lhs == rhs) || (lhs < rhs));
}

bool CNum::operator<=(const Counter &lhs, const Counter &rhs) {
  return (lhs < rhs || lhs == rhs);
}

bool CNum::operator>=(const Counter &lhs, const Counter &rhs) {
  return !(lhs < rhs);
}

unsigned long long CNum::Counter::ull() const {
  assert(sizeof(unsigned long long) <= sizeof(Unit));
  return value.size() ? value.front() : 0;
}

CNum::Unit CNum::Counter::unit() const { return value.size() ? value[0] : 0; }

CNum::Counter &CNum::Counter::operator=(const Counter &rhs) {
  value = rhs.value;
  return *this;
}

CNum::Counter &CNum::Counter::operator+=(const Counter &rhs) {
  assert(isNormalized());
  assert(rhs.isNormalized());

  Unit carry = 0;
  for (Index pos = 0; carry || pos < rhs.value.size(); ++pos) {
    if (pos >= value.size()) {
      value.push_back(0);
    }
    Unit r = pos >= rhs.value.size() ? 0 : rhs.value[pos];
    CNum::add(value[pos], r, carry);
  }
  assert(isNormalized());
  return *this;
}

CNum::Counter &CNum::Counter::operator-=(const Counter &rhs) {
  assert(isNormalized());
  assert(rhs.isNormalized());
  assert(rhs <= *this);

  Unit borrow = 0;
  for (Index pos = 0; borrow || pos < rhs.value.size(); ++pos) {
    Unit r = pos >= rhs.value.size() ? 0 : rhs.value[pos];
    CNum::sub(value[pos], r, borrow);
  }
  assert(isNormalized());
  return *this;
}

CNum::Counter &CNum::Counter::operator*=(const Counter &rhs) {
  assert(isNormalized());
  assert(rhs.isNormalized());

  Unit carry = 0;
  for (Index pos = 0; carry || pos < rhs.value.size(); ++pos) {
    if (pos >= value.size()) {
      value.push_back(0);
    }
    Unit r = pos >= rhs.value.size() ? 0 : rhs.value[pos];
    CNum::mul(value[pos], r, carry);
  }
  normalize();
  assert(isNormalized());
  return *this;
}

CNum::Counter CNum::operator+(Counter lhs, const Counter &rhs) {
  return lhs += rhs;
}

CNum::Counter CNum::operator-(Counter lhs, const Counter &rhs) {
  return lhs -= rhs;
}

CNum::Counter CNum::operator*(Counter lhs, const Counter &rhs) {
  return lhs *= rhs;
}

CNum::Counter &CNum::Counter::operator++() {
  *this += 1;
  return *this;
}

CNum::Counter CNum::Counter::operator++(int) {
  Counter c(*this);
  ++(*this);
  return c;
}

CNum::Counter CNum::Counter::operator+() const { return *this; }
/*
CNum::Counter& CNum::Counter::operator<<=(const Counter& rhs) {
Unit shift = rhs.unit();
return rhs;
}
*/

// CNum::Counter::Counter(const std::string s) :
//		m_size(0), m_ptr(nullptr) {
/*
 auto rItr = s.rbegin();
 for (; rItr != s.rend(); ++rItr) {
 char c = *rItr;
 if (!('0' <= c && c <= '1')) {
 break;
 }
 }
 byte_size size = rItr - s.rbegin();
 zero(size/8+1);
 for (; rItr != s.rbegin(); --rItr) {
 *this <<= 1ULL;
 *this += *rItr - '0';
 }
 byte b = 0;
 byte_pos bitPos = 0;
 byte_pos bytePos = 0;
 for (auto itr = s.rbegin(); itr != s.rend(); ++itr) {
 b |= ( == '0' ? 0 : 1) << bitPos++;
 if (bitPos >= 8) {
 []
 }
 }
 */
//}
/*
 CNum::Counter& CNum::Counter::operator <<(const Counter&) {
 return *this;
 }
 */

/*
 CNum::Counter::byte_pos_diff CNum::Counter::diff(byte_pos i, byte_pos j) {
 bool i_larger = i > j;
 byte_size abs_diff = i_larger ? i - j : j - i;
 assert(abs_diff <= INT64_MAX);
 byte_pos_diff diff = static_cast<byte_pos_diff>(abs_diff);
 return i_larger ? diff : -diff;
 }
 */
/*
 CNum::Counter& CNum::Counter::operator --() {
 byte_pos i = 0;
 while (m_ptr[i] == 0) {
 assert(i != m_size - 1);
 --m_ptr[i];
 ++i;
 }
 --m_ptr[i];
 return *this;
 }
 */

/*
 CNum::Counter CNum::Counter::operator --(int) {
 Counter c(*this);
 --(*this);
 return c;
 }
 */

/*
 void CNum::Counter::resize(byte_size s) {
 if (!(m_size < s || (m_size >= s * SIZE_SHRINK_FACTOR))) {
 return;
 }

 const byte_size lastSize = m_size;
 std::unique_ptr<byte[]> pLastPtr = std::move(m_ptr);

 m_size = s;
 m_ptr.reset(new byte[m_size]());

 std::copy_n(pLastPtr.get(), std::min(lastSize, m_size), m_ptr.get());
 }
 */

/*
 inline void CNum::Counter::expand() {
 resize(m_size * SIZE_EXPAND_FACTOR);
 }
 */
/*
 void CNum::Counter::addOne(byte_pos i) {
 while (m_ptr[i] == 0xFF) {
 ++m_ptr[i];
 if (i == m_size - 1) {
 expand();
 }
 ++i;
 }
 ++m_ptr[i];
 }
 */
