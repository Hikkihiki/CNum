# Seemless
Seemless is yet another big integer library for C++ with foucus on its seemless interface such that big integer class could be used as if it is just a primitive type.

## Design Goal
The design focus is to provide a seemless way to use big integer with a close to zero learning curve.

## To do list
- [x] Default constructor
- [x] Copy constructor
- [x] Constructor by hexadecimal string
- [x] Constructor by decimal string
- [ ] Constructor by binary string
- [x] Automatic memory reallocation (by using STL)
- [x] Explicit trim and downcast to unsigned long long
- [ ] Implicit cast to bool
- [x] Addition: +, +=
- [x] Subtraction: -, -=
- [x] Muplication: *, *=
- [x] Division: /, /=
- [x] Mod: %, %=
- [x] Exponential: pow()
- [x] Comparision: ==, <, >, >=, <= 
- [ ] Bitwise Operation: &, &=, |, |=, ^, ^=
- [x] Bit shifting <<, <<=, >>, >>=
- [ ] Output to std::cout
- [ ] Input from std::cin
- [ ] Literal like 123456ULL that append after digit to create a big integer type (is this possible by macro?) 
