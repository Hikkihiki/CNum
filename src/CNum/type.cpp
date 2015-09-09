#include "type.hpp"

CNum::Z::Z(const long & v) :
		m_value(v < 0 ? -v : v), m_positive(v >= 0) {
}

bool CNum::Z::operator ==(const Z& rhs) const {
	return (m_value == rhs.m_value)
			&& ((m_value == 0) || m_positive == rhs.m_positive);
}

bool CNum::Z::operator !=(const Z& rhs) const {
	return !(*this == rhs);
}

bool CNum::Z::operator <(const Z& rhs) const {
	bool same_sign = m_positive == rhs.m_positive;
	bool less_than = m_value < rhs.m_value;
	return !(*this == rhs)
			&& (!(same_sign || m_positive)
					|| (same_sign && (m_positive == less_than)));
}

bool CNum::Z::operator >(const Z& rhs) const {
	return !(*this < rhs || *this == rhs);
}

bool CNum::Z::operator <=(const Z& rhs) const {
	return (*this < rhs) || (*this == rhs);
}

bool CNum::Z::operator >=(const Z& rhs) const {
	return !(*this < rhs) || (*this == rhs);
}

CNum::Z CNum::Z::operator -() const {
	Z rv = *this;
	rv.m_positive = !rv.m_positive;
	return rv;
}

CNum::Z CNum::Z::operator +() const {
	return *this;
}
