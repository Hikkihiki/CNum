#include "N.hpp"

#include <assert.h>
/*
CNum::N(const long& v) {

}


bool CNum::N::operator ==(const Z& rhs) const {

}
*/

CNum::Value::Value() :
		Value(0) {
}

CNum::Value::Value(const long & v) :
		m_byteSize(0), m_pBytes(nullptr) {
	m_byteSize = sizeof(v) * 2;
	m_pBytes = new Byte[m_byteSize]();
	if(v == 0)
		return;

	const unsigned long t = v < 0 ? -v : v;
	assert(sizeof(t) <= m_byteSize);
	memcpy(m_pBytes, &t, sizeof(t));
}

CNum::Value::~Value() {
	delete[] m_pBytes;
}
