#pragma once
#include "fraction.h"

class FractComparison {
public:
    static bool equals(const Fraction& a, const Fraction& b);
    static bool lessThan(const Fraction& a, const Fraction& b);
    static bool greaterThan(const Fraction& a, const Fraction& b);
};