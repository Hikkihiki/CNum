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


	Counter& operator +=(const Counter& rhs);
	Counter& operator <<(const Counter& rhs);

private:
	typedef unsigned char byte;
	typedef unsigned long byte_size;
	typedef byte_size byte_pos;

	void zero(byte_size s);
	void resize(byte_size s);

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
