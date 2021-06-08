#ifndef ___VECTORS_TP_PJATK_HPP___
#define ___VECTORS_TP_PJATK_HPP___
#include <array>
#include <cmath>

namespace tp::operators {

template<typename T1,typename T2>
inline T1 operator+(const T1& a, const T2& b) {
    T1 ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] += b[i];
    return ret;
}

inline std::array<double,2> operator*(const std::array<double,2>& a, const std::array<double,2>& b) {
    std::array<double,2> ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] *= b[i];
    return ret;
}

inline std::array<double,2> operator*(const std::array<double,2>& a, double b) {
    std::array<double,2> ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] *= b;
    return ret;
}

inline std::array<double,2> operator/(const std::array<double,2>& a, double b) {
    std::array<double,2> ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] /= b;
    return ret;
}

template<typename T1,typename T2>
inline T1 operator-(const T1& a, const T2& b) {
    T1 ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] -= b[i];
    return ret;
}

template<typename T>
inline double length(const T& a) {
    double ret = 0.0;
    for (unsigned i = 0; i < a.size();i++) ret+= a[i] * a[i];
    return (ret == 0)?0.0:std::sqrt(ret);
}

}; // namespace tp


#endif
