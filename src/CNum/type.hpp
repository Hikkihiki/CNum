#ifndef SRC_TYPE_HPP_
#define SRC_TYPE_HPP_

namespace CNum {

class INumericType {

};

class Z {
public:
	Z(const long& v);
	bool operator ==(const Z& rhs) const;
	bool operator !=(const Z& rhs) const;
private:
	long m_v;
};

}

#endif /* SRC_CNUM_TYPE_HPP_ */
