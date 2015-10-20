#ifndef INCLUDE_COUNTER_HPP_
#define INCLUDE_COUNTER_HPP_

#include <memory>
#include <string>
#include <cstdint>
#include <vector>

namespace CNum {

// To be refactored...
// A unit is defined to be unsigned primitive type
// It should have implicit conversion to unsigned long long
typedef uint64_t Unit;
typedef int64_t Index;
typedef uint64_t Size;

const Unit UNIT_MAX = -1;
// const Unit UNIT_MIN = 0;

// unit1 = unit1 + unit2 + carry, carry = new carry
void add(Unit& unit1, const Unit& unit2, Unit& carry);

// unit1 = unit1 - unit2 - carry, carry = new carry
void subtract(Unit& unit1, const Unit& unit2, Unit& carry);

// unit = (unit << shift) | filler, filler = dropped bits
void left_shift(Unit& unit, const Unit& shift, Unit& filler);

class Counter {
 public:
  // typedef unsigned long UL;

  Counter();
  // Counter(const std::string s);
  Counter(unsigned long long v);

  // copy constructor
  Counter(const Counter& c);

  // copy assignment
  Counter& operator=(const Counter& rhs);

  // == operator is declared as free function so that
  // 1 == Counter(1) is valid expression
  friend bool operator==(const Counter&, const Counter&);
  friend bool operator!=(const Counter&, const Counter&);
  friend bool operator<(const Counter&, const Counter&);
  friend bool operator>(const Counter&, const Counter&);
  friend bool operator<=(const Counter&, const Counter&);
  friend bool operator>=(const Counter&, const Counter&);

  // prefix
  Counter& operator++();
  // Counter& operator --();

  // postfix
  Counter operator++(int);
  // Counter operator --(int);

  // prefix
  Counter& operator-() = delete;
  Counter operator+() const;

  Counter& operator+=(const Counter& rhs);
  Counter& operator-=(const Counter& rhs);

  // Counter& operator <<(const Counter& rhs);
  // Counter& operator<<=(const Counter& rhs);

  // Down Casting
  unsigned long long ull() const;
  Unit unit() const;

 private:
  // typedef std::size_t byte_size;
  // typedef byte_size byte_pos;
  // typedef std::ptrdiff_t byte_pos_diff;

  void setZero();
  // void resize(byte_size s);
  // void expand();

  void normalize();
  bool isNormalized() const;

  // void addOne(byte_pos i);

  // static byte_pos_diff diff(byte_pos i, byte_pos j);

  // byte_size m_size;

  // always assert that value is normalize,
  // I.e. no leading zeros
  // zero is represent by zero size vector
  std::vector<Unit> value;
  // std::unique_ptr<Unit[]> m_ptr;
};

Counter operator+(Counter lhs, Counter const& rhs);
Counter operator-(Counter lhs, Counter const& rhs);
bool operator==(const Counter&, const Counter&);
bool operator!=(const Counter&, const Counter&);
bool operator<(const Counter&, const Counter&);
bool operator>(const Counter&, const Counter&);
bool operator<=(const Counter&, const Counter&);
bool operator>=(const Counter&, const Counter&);
}

#endif /* INCLUDE_COUNTER_HPP_ */
