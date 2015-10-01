#include "../include/counter.hpp"

#include <algorithm>
#include <memory>
#include <cassert>

#define SIZE_SHRINK_FACTOR (16)
#define SIZE_EXPAND_FACTOR (2)

/*
 CNum::N(const long& v) {

 }


 bool CNum::N::operator ==(const Z& rhs) const {

 }
 */

CNum::Counter::Counter() :
		Counter(0) {
}

CNum::Counter::Counter(const Counter& c) :
		Counter(0) {
	*this = c;
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

CNum::Counter& CNum::Counter::operator =(const Counter& rhs) {
	resize(rhs.m_size);
	std::copy_n(rhs.m_ptr.get(), m_size, m_ptr.get());
	return *this;
}

CNum::Counter& CNum::Counter::operator <<(const Counter&) {
	return *this;
}

bool CNum::Counter::operator ==(const Counter& rhs) const {
	const Counter* large = this;
	const Counter* small = &rhs;
	if (m_size < rhs.m_size) {
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

CNum::Counter& CNum::Counter::operator ++() {
	addOne(0);
	return *this;
}

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

CNum::Counter CNum::Counter::operator ++(int) {
	Counter c(*this);
	++(*this);
	return c;
}

CNum::Counter CNum::Counter::operator --(int) {
	Counter c(*this);
	--(*this);
	return c;
}

CNum::Counter& CNum::Counter::operator +=(const Counter& rhs) {
	bool carry = 0;
	byte_pos i = 0;
	for (; i < rhs.m_size; i++) {
		if (m_size < i) {
			expand();
		}
		unsigned long sum = m_ptr[i] + rhs.m_ptr[i] + (carry ? 1 : 0);
		m_ptr[i] = (Counter::byte) sum;
		carry = (sum >> 8) > 0;
	}
	if (carry) {
		addOne(i);
	}
	return *this;
}

CNum::Counter& CNum::Counter::operator +() {
	return *this;
}

/*
 CNum::Counter::operator CNum::Counter::UL() const {
 UL value;
 return value;
 }
 */

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

void CNum::Counter::zero(byte_size s) {
	if (s == m_size) {
		std::fill_n(m_ptr.get(), m_size, 0);
	} else {
		m_size = s;
		m_ptr.reset(new byte[m_size]());
	}
}

CNum::Counter CNum::operator +(Counter lhs, const Counter& rhs) {
	return lhs += rhs;
}

inline void CNum::Counter::expand() {
	resize(m_size * SIZE_EXPAND_FACTOR);
}

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
