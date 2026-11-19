#include "../include/fractcomparison.h"

bool FractComparison::equals(const Fraction& a, const Fraction& b) {
    return (static_cast<long long>(a.getNumerator()) * b.getDenominator()) ==
        (static_cast<long long>(b.getNumerator()) * a.getDenominator());
}

bool FractComparison::lessThan(const Fraction& a, const Fraction& b) {
    return (static_cast<long long>(a.getNumerator()) * b.getDenominator()) <
        (static_cast<long long>(b.getNumerator()) * a.getDenominator());
}

bool FractComparison::greaterThan(const Fraction& a, const Fraction& b) {
    return lessThan(b, a);
}