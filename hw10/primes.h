#pragma once

#include <cmath>

template <typename T> constexpr auto gcd(T a, T b) { // 2 arguments
    while (b!=0) {
        T c = a % b;
        a = b;
        b = c;
    }
    return a;
}

template <typename T, typename... Args> constexpr auto gcd(T a, T b, Args... args) { // more than 2 arguments
    return gcd(gcd(a, b), args...);
}

template <typename T> constexpr auto mcm(T a, T b) {
    return a*b/gcd(a, b);
}

template <typename T, typename... Args> constexpr auto mcm(T a, T b, Args... args) {
    return mcm(mcm(a, b), args...);
}

template <int base, int exponent, int modulus> struct Power {
    static constexpr int value = (Power<base, exponent-1, modulus>::value*base)%modulus;
};

template<int base, int modulus> struct Power<base, 0, modulus> {
    static constexpr int value = 1%modulus;
};



