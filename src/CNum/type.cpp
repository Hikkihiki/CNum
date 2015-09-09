#include "type.hpp"

CNum::Z::Z(const long & v) :
		m_v(v) {
}

bool CNum::Z::operator ==(const Z& rhs) const {
	return m_v == rhs.m_v;
}

bool CNum::Z::operator !=(const Z& rhs) const {
	return !(*this == rhs);
}
