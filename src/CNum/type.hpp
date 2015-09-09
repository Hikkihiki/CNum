#ifndef SRC_TYPE_HPP_
#define SRC_TYPE_HPP_

namespace CNum {

typedef unsigned long Unit;
typedef bool Sign;

class Z {

public:
	Z(const long& v);

	bool operator ==(const Z& rhs) const;
	bool operator !=(const Z& rhs) const;

	bool operator <(const Z&rhs) const;
	bool operator >(const Z&rhs) const;

	bool operator <=(const Z&rhs) const;
	bool operator >=(const Z&rhs) const;

	Z operator-() const;
	Z operator+() const;
private:
	Unit m_value;
	Sign m_positive;
};

}

#endif /* SRC_CNUM_TYPE_HPP_ */
