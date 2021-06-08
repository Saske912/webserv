#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <iostream>
#include <sstream>

template<class T>
class NumericLimits {
    typedef T type;
public:
    static const int byte_size = 8;
    static const bool is_signed = type(-1) < type(0);
    static const int digits2 = sizeof(type) * byte_size - 1;
    static const type min = type(type(1) << digits2);
    static const type max = is_signed ? type(~0 ^ min) : type(~0);
};

template<class Type>
bool strtot(const char *str, Type &value) {
    const Type remainderLimit = NumericLimits<Type>::max / 10;
    const Type limitLastDigit = NumericLimits<Type>::max % 10;
    int  sign = 1;

    value = 0;
    while (*str == ' ' || (0x09 <= *str && *str <= 0x0d ))
        str++;
    if (*str == '-' || *str == '+')
        sign = (*(str++) == '-') ? -1 : +1;
    while (*str) {
        if (*str < '0' || *str > '9')
            break ;
        if (value > remainderLimit ||
            (value == remainderLimit && *str - (sign < 0) - '0' > limitLastDigit))
            return false;
        value = value * 10 + (*str - '0');
        str++;
    }
    value *= sign;
    return true;
}

// might recursibely call itself until segv :cry:
template<class Type>
bool strtot(const std::string &str, Type &value) {
    return strtot<Type>(str.c_str(), value);
}


template<class Type>
std::string ttostr(Type value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

#endif
