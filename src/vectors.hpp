#ifndef ___VECTORS_TP_PJATK_HPP___
#define ___VECTORS_TP_PJATK_HPP___
#include <array>
#include <cmath>

namespace tp::operators {

//Addition
template<typename T1,typename T2>
inline T1 operator+(const T1& a, const T2& b) {
    T1 ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] += b[i];
    return ret;
}

//Multiplication
//arr<double> * arr<double>
inline std::array<double,2> operator*(const std::array<double,2>& a, const std::array<double,2>& b) {
    std::array<double,2> ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] *= b[i];
    return ret;
}

//arr<double> * double
inline std::array<double,2> operator*(const std::array<double,2>& a, double b) {
    std::array<double,2> ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] *= b;
    return ret;
}

//arr<double> * int
inline std::array<double,2> operator*(const std::array<double,2>& a, int b) {
    std::array<double,2> ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] *= b;
    return ret;
}

//arr<int> * int
inline std::array<int,2> operator*(const std::array<int,2>& a, int b) {
    std::array<int,2> ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] *= b;
    return ret;
}

//Division
inline std::array<double,2> operator/(const std::array<double,2>& a, double b) {
    std::array<double,2> ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] /= b;
    return ret;
}

//Liniar interpolation
inline double lerp(const int& a, const int& b, double c) {
    return a + c * (b - a);
}


//Subtraction
template<typename T1,typename T2>
inline T1 operator-(const T1& a, const T2& b) {
    T1 ret = a;
    for (unsigned i = 0; i < a.size();i++) ret[i] -= b[i];
    return ret;
}

//DOT product
template<typename T1,typename T2>
inline double dot(const T1& a, const T2& b) {
    double ret = 0.0;
    for (unsigned i = 0; i < a.size();i++) ret += a[i] * b[i];
    return (ret == 0)?0.0:ret;
}

//Magnitude
template<typename T>
inline double length(const T& a) {
    double ret = 0.0;
    for (unsigned i = 0; i < a.size();i++) ret += a[i] * a[i];
    return (ret == 0)?0.0:std::sqrt(ret);
}

//Normalized
template<typename T>
inline std::array<double,2> normalize(const T& a) {
    std::array<double,2> ret = {0, 0};
    double magnitude = 0.0;
    for (unsigned i = 0; i < a.size();i++) magnitude += a[i] * a[i];
    magnitude = sqrt(magnitude);
    for (unsigned i = 0; i < a.size();i++) ret[i] += a[i] / magnitude;
    return ret;
}

//Clamp
template<typename T>
constexpr T clamp(const T& v, const T& lo, const T& hi){
   return v > hi ? hi : v < lo ? lo : v;
}

}; // namespace tp


#endif
