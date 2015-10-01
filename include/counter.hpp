#ifndef INCLUDE_COUNTER_HPP_
#define INCLUDE_COUNTER_HPP_

#include <memory>

namespace CNum {

class Counter {

public:
	typedef unsigned long UL;

	Counter();
	Counter(const UL& v);

	// copy constructor
	Counter(const Counter& c);

	// copy assignment
	Counter& operator =(const Counter& rhs);

	//explicit operator UL() const;

	bool operator ==(const Counter& rhs) const;

	// prefix
	Counter& operator ++();
	Counter& operator --();

	// postfix
	Counter operator ++(int);
	Counter operator --(int);

	// prefix
	Counter& operator -() = delete;
	Counter& operator +();

	Counter& operator +=(const Counter& rhs);
	Counter& operator <<(const Counter& rhs);

	// Explicit Conversion
	unsigned long long ull();

private:
	typedef unsigned char byte;
	typedef std::size_t byte_size;	
	typedef byte_size byte_pos;
	typedef std::ptrdiff_t byte_pos_diff;

	void zero(byte_size s);
	void resize(byte_size s);
	void expand();

	void addOne(byte_pos i);

	static byte_pos_diff diff(byte_pos i, byte_pos j);

	byte_size m_size;
	std::unique_ptr<byte[]> m_ptr;
};

Counter operator+(Counter lhs, Counter const& rhs);

/*
 class N {

 public:
 N(const long& v);

 bool operator ==(const Z& rhs) const;

 private:

 vector<Unit> m_value;
 };
 */

}

#endif /* INCLUDE_COUNTER_HPP_ */
