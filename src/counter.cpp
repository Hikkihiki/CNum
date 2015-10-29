#include "../include/counter.hpp"

#include <algorithm>
#include <cassert>
#include <sstream>

const CNum::Unit HEX_CHAR_SIZE = 4;

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
  assert(0 <= shift && shift < UNIT_BIT_SIZE);
  Unit newFiller = unit >> (UNIT_BIT_SIZE - shift);
  unit <<= shift;
  unit |= filler;
  filler = newFiller;
}

void CNum::right_shift(Unit &unit, const Unit &shift, Unit &filler) {
  assert(0 <= shift && shift < UNIT_BIT_SIZE);
  Unit newFiller = unit << (UNIT_BIT_SIZE - shift);
  unit >>= shift;
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
      shift = (shift + HEX_CHAR_SIZE) % UNIT_BIT_SIZE;
      if (shift == 0) {
        value.push_back(unit);
        unit = 0;
      }
    }
    value.push_back(unit);
  } else if (base == 10) {
    // Naive algorithm that make use of multiplication and addition
    *this = 0;
    Counter exp = 1;
    for (auto itr = rBegin; itr != rEnd; itr++, exp *= base) {
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

  normalize();
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

CNum::Unit CNum::Counter::operator[](const Index &idx) const {
  assert(0 <= idx && idx < value.size());
  return value[idx];
}

// Simply use printf?
std::string CNum::Counter::hex() const {
  assert(isNormalized());
  assert(UNIT_BIT_SIZE % HEX_CHAR_SIZE == 0);
  const Unit CHAR_NUM = UNIT_BIT_SIZE / HEX_CHAR_SIZE;
  const Unit filter = 0xF;
  std::string rv("0x");
  for (auto itr = value.crbegin(); itr != value.crend(); ++itr) {
    for (int i = 0; i < CHAR_NUM; ++i) {
      Unit shift = HEX_CHAR_SIZE * (CHAR_NUM - i - 1);
      Unit u = (*itr & (filter << shift)) >> shift;
      if (u <= 9) {
        rv.push_back(u + '0');
      } else {
        assert(0xA <= u && u <= 0xF);
        rv.push_back(u - 10 + 'A');
      }
    }
  }
  while (rv.size() >= 4 && rv[2] == '0') {
    rv.erase(rv.begin() + 2, rv.begin() + 3);
  }
  assert(rv == "0x0" || rv[2] != '0');
  return rv;
}

// Implement with long division
std::string CNum::Counter::dec() const {
  assert(isNormalized());
  std::string rv("");
  return rv;
}

CNum::Unit CNum::Counter::size() const {
  assert(isNormalized());
  return value.size();
}

CNum::Unit CNum::Counter::bitSize() const {
  assert(isNormalized());
  assert(value.size() >= 1);
  Unit filter = 0x1;
  Unit count = 0;
  for (Unit i = UNIT_BIT_SIZE; i > 0; --i) {
    if (value[value.size() - 1] & (filter << (i - 1))) {
      count = i;
      break;
    }
  }
  return count + (value.size() - 1) * UNIT_BIT_SIZE;
}

CNum::Counter &CNum::Counter::operator=(const Counter &rhs) {
  value = rhs.value;
  return *this;
}

// Doing in place computation could be dangerous...
CNum::Counter &CNum::Counter::operator+=(const Counter &rhs) {
  assert(isNormalized());
  assert(rhs.isNormalized());

  Unit carry = 0;
  for (Index pos = 0; carry || pos < rhs.value.size(); ++pos) {
    if (pos >= value.size()) {
      value.push_back(0);
    }
    Unit r = (pos >= rhs.value.size() ? 0 : rhs.value[pos]);
    CNum::add(value[pos], r, carry);
  }
  assert(isNormalized());
  return *this;
}

// Doing in place computation could be dangerous...
CNum::Counter &CNum::Counter::operator-=(const Counter &rhs) {
  assert(isNormalized());
  assert(rhs.isNormalized());
  assert(rhs <= *this);

  Unit borrow = 0;
  for (Index pos = 0; borrow || pos < rhs.value.size(); ++pos) {
    Unit r = pos >= rhs.value.size() ? 0 : rhs.value[pos];
    CNum::sub(value[pos], r, borrow);
  }
  normalize();
  assert(isNormalized());
  return *this;
}

CNum::Counter &CNum::Counter::operator*=(const Counter &rhs) {
  assert(isNormalized());
  assert(rhs.isNormalized());

  Counter sol = 0;
  Unit shift1 = 0;
  for (Index rpos = 0; rpos < rhs.value.size(); ++rpos) {
    Unit shift2 = 0;
    Unit carry = 0;
    for (Index lpos = 0; carry || lpos < value.size(); ++lpos) {
      Unit l = lpos < value.size() ? value[lpos] : 0;
      CNum::mul(l, rhs.value[rpos], carry);
      sol += (Counter(l) << (shift1 + shift2));
      shift2 += UNIT_BIT_SIZE;
    }
    shift1 += UNIT_BIT_SIZE;
  }
  assert(sol.isNormalized());
  *this = sol;
  assert(isNormalized());
  return *this;
}

// Doing division by naive search... O(n^3)
CNum::Counter &CNum::Counter::operator/=(const Counter &rhs) {
  assert(isNormalized());
  assert(rhs.isNormalized());
  assert(rhs > 0);
  // find upperbound ub such that rhs*ub > lhs >= rhs*(ub/2)
  Counter rem(*this);
  Counter sol = 0;
  while (rem >= rhs) {
    Counter bit(1);
    Counter ub(rhs);
    while (ub <= rem) {
      ub <<= 1;
      bit <<= 1;
    }
    ub >>= 1;
    bit >>= 1;
    sol += bit;
    rem -= ub;
  }
  *this = sol;
  assert(isNormalized());
  return *this;
}

CNum::Counter &CNum::Counter::operator%=(const Counter &rhs) {
  assert(isNormalized());
  assert(rhs.isNormalized());
  assert(rhs > 0);
  *this -= (*this / rhs * rhs);
  assert(isNormalized());
  return *this;
}

CNum::Counter CNum::Counter::pow(const Counter &rhs) const {
  assert(isNormalized());
  if (*this == 0 && rhs == 0)
    throw;
  if (*this == 0)
    return 0;
  if (rhs == 0)
    return 1;
  // *this >=1 && rhs >=1 here
  // Do repeated square algorithm
  const Unit SQ_SIZE = rhs.bitSize();
  assert(SQ_SIZE >= 1);
  std::vector<Counter> sq(SQ_SIZE);
  sq[0] = *this;
  for (Unit i = 1; i < SQ_SIZE; ++i) {
    sq[i] = sq[i - 1] * sq[i - 1];
  }
  Counter rv(1);
  for (Unit i = 0; i < SQ_SIZE; ++i) {
    if (rhs.isSet(i)) {
      rv *= sq[i];
    }
  }
  return rv;
}

CNum::Counter CNum::Counter::log2() const {
  assert(isNormalized());
  if (*this == 0) {
    throw;
  }
  return bitSize() - 1;
}

bool CNum::Counter::isSet(const Unit &idx) const {
  assert(isNormalized());
  if (idx >= bitSize()) {
    return false;
  }
  Unit div = idx / CNum::UNIT_BIT_SIZE;
  Unit rem = idx % CNum::UNIT_BIT_SIZE;
  const Unit FILTER = 1;
  return (value[div] & (FILTER << rem));
}

CNum::Counter &CNum::Counter::operator<<=(const Counter &rhs) {
  assert(rhs.value.size() == 1);
  if (*this == 0) {
    return *this;
  }

  const Unit div = rhs[0] / UNIT_BIT_SIZE;
  const Unit rem = rhs[0] % UNIT_BIT_SIZE;

  // fine shift
  if (rem) {
    Unit filler = 0;
    for (Unit i = 0; i < value.size(); ++i) {
      CNum::left_shift(value[i], rem, filler);
    }
    if (filler) {
      value.push_back(filler);
    }
  }
  assert(isNormalized());
  // coarse shift
  for (Unit i = 0; i < div; ++i) {
    value.insert(value.cbegin(), 0);
  }
  assert(isNormalized());
  return *this;
}

CNum::Counter &CNum::Counter::operator>>=(const Counter &rhs) {
  assert(rhs.value.size() == 1);
  if (*this == 0) {
    return *this;
  }
  const Unit div = rhs[0] / UNIT_BIT_SIZE;
  const Unit rem = rhs[0] % UNIT_BIT_SIZE;
  // coarse shift
  for (Unit i = 0; i < div; ++i) {
    value.erase(value.begin());
  }
  assert(isNormalized());
  // fine shift
  if (rem) {
    Unit filler = 0;
    for (Unit i = value.size(); i > 0; --i) {
      CNum::right_shift(value[i - 1], rem, filler);
    }
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

CNum::Counter CNum::operator/(Counter lhs, const Counter &rhs) {
  return lhs /= rhs;
}

CNum::Counter CNum::operator%(Counter lhs, const Counter &rhs) {
  return lhs %= rhs;
}

CNum::Counter CNum::operator<<(Counter lhs, const Counter &rhs) {
  return lhs <<= rhs;
}

CNum::Counter CNum::operator>>(Counter lhs, const Counter &rhs) {
  return lhs >>= rhs;
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
