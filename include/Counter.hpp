#ifndef INCLUDE_COUNTER_HPP_
#define INCLUDE_COUNTER_HPP_

#include <vector>
#include <memory>

namespace CNum {

class Counter {

public:
	typedef unsigned long UL;

	Counter();
	Counter(const UL& v);
	Counter(const Counter& c); // copy constructor

	Counter& operator =(const Counter& c); // copy assignment

	bool operator ==(const Counter& c) const;
	Counter& operator <<(const Counter& c);

private:
	typedef unsigned char byte;
	typedef unsigned long byte_size;
	typedef byte_size byte_pos;

	void zero(byte_size s);
	//void resize(byte_size s);

	byte_size m_size;
	std::unique_ptr<byte[]> m_ptr;
};

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
