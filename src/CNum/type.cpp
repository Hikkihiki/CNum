#include "type.hpp"

CNum::Int::Int(const long & v) :
		m_v(v) {
}

bool CNum::Int::operator ==(const Int& rhs) const {
	return m_v == rhs.m_v;
}

bool CNum::Int::operator !=(const Int& rhs) const {
	return !(*this == rhs);
}
