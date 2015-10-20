#include "../include/counter.hpp"

#include <algorithm>
#include <cassert>

void CNum::add(Unit& unit1, const Unit& unit2, Unit& carry) {
  assert(carry == 0 || carry == 1);
  carry = (unit1 += carry) < carry;
  carry += (unit1 += unit2) < unit2;
}

void CNum::left_shift(Unit& unit, const Unit& shift, Unit& filler) {
  assert(0 <= shift && shift < sizeof(Unit) * 8);
  Unit newFiller = unit >> (sizeof(Unit) * 8 - shift);
  unit <<= shift;
  unit |= filler;
  filler = newFiller;
}

CNum::Counter::Counter(unsigned long long v) : value() {
  assert(sizeof(unsigned long long) <= sizeof(Unit));
  if (v) {
    value.push_back(v);
  }
  assert(isNormalized());
}

CNum::Counter::Counter() : Counter(0) {}

CNum::Counter::Counter(const Counter& c) : Counter(0) { *this = c; }

void CNum::Counter::setZero() {
  value.clear();
  assert(isNormalized());
}

bool CNum::Counter::isNormalized() const {
  return value.size() ? (value.back() != 0) : true;
}

void CNum::Counter::normalize() {
  // trim leading zero
  while (!value.back()) {
    value.pop_back();
  }
  assert(isNormalized());
}

bool CNum::operator==(const Counter& lhs, const Counter& rhs) {
  assert(lhs.isNormalized());
  assert(rhs.isNormalized());
  return lhs.value == rhs.value;
}

unsigned long long CNum::Counter::ull() const {
  assert(sizeof(unsigned long long) <= sizeof(Unit));
  return value.size() ? value.front() : 0;
}

CNum::Unit CNum::Counter::unit() const { return value.size() ? value[0] : 0; }

CNum::Counter& CNum::Counter::operator=(const Counter& rhs) {
  value = rhs.value;
  return *this;
}

CNum::Counter& CNum::Counter::operator+=(const Counter& rhs) {
  assert(isNormalized());
  assert(rhs.isNormalized());

  Unit carry = 0;
  UnitPos pos = 0;
  for (; carry || pos < rhs.value.size(); ++pos) {
    if (pos >= value.size()) {
      value.push_back(0);
    }
    Unit r = pos >= rhs.value.size() ? 0 : rhs.value[pos];
    CNum::add(value[pos], r, carry);
  }
  assert(isNormalized());
  return *this;
}

CNum::Counter CNum::operator+(Counter lhs, const Counter& rhs) {
  return lhs += rhs;
}

CNum::Counter& CNum::Counter::operator++() {
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
