#ifndef SRC_TYPE_HPP_
#define SRC_TYPE_HPP_

namespace CNum {

class INumericType {

};

class Int {
public:
	Int(const long& v);
	bool operator ==(const Int& rhs) const;
	bool operator !=(const Int& rhs) const;
private:
	long m_v;
};

}

#endif /* SRC_CNUM_TYPE_HPP_ */
