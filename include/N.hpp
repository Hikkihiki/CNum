#ifndef INCLUDE_N_HPP_
#define INCLUDE_N_HPP_

#include <vector>
using std::vector;

namespace CNum {

typedef unsigned char Byte;
typedef unsigned long Size;

class Value {

public:
	Value();
	Value(const long& v);
	~Value();

private:
	Size m_byteSize;
	Byte* m_pBytes;
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

#endif /* INCLUDE_N_HPP_ */
