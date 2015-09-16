#include "../include/Counter.hpp"

#include <cassert>
#include <algorithm>
/*
 CNum::N(const long& v) {

 }


 bool CNum::N::operator ==(const Z& rhs) const {

 }
 */

CNum::Counter::Counter() :
		Counter(0) {
}

CNum::Counter::Counter(const UL & v) :
		m_size(0), m_ptr(nullptr) {
	zero(sizeof(UL));
	UL ul = v;
	byte_pos i = 0;
	while (ul > 0) {
		m_ptr[i] = (ul & 0xFF);
		i++;
		ul >>= 8;
	}
}

CNum::Counter::Counter(const Counter& c) {
	*this = c;
}

CNum::Counter& CNum::Counter::operator =(const Counter& c) {
	zero(c.m_size);
	std::copy_n(c.m_ptr.get(), m_size, m_ptr.get());
	return *this;
}

CNum::Counter& CNum::Counter::operator <<(const Counter& c) {
	return *this;
}

bool CNum::Counter::operator ==(const Counter& c) const {
	const Counter* large = this;
	const Counter* small = &c;
	if (m_size < c.m_size) {
		std::swap(large, small);
	}

	if (!std::equal(small->m_ptr.get(), small->m_ptr.get() + small->m_size,
			large->m_ptr.get())) {
		return false;
	}
	if (std::count(large->m_ptr.get() + small->m_size,
			large->m_ptr.get() + large->m_size, 0)
			== (large->m_size - small->m_size))
		return true;
	return false;
}

/*
 void CNum::Counter::resize(byte_size s) {
 if (s == m_size) {
 return;
 }

 const byte_size lastSize = m_size;
 std::unique_ptr<byte> pLastPtr() = m_ptr;

 m_size = s;
 m_ptr = new byte[m_size]();

 byte* ptr;
 std::copy_n(pLastPtr, std::min(lastSize, m_size), m_ptr);
 }
 */

void CNum::Counter::zero(byte_size s) {
	if (s == m_size) {
		std::fill_n(m_ptr.get(), m_size, 0);
	} else {
		m_size = s;
		m_ptr.reset(new byte[m_size]());
	}
}
