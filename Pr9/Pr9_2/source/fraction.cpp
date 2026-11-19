#include "../include/fraction.h"
#include <stdexcept>

Fraction::Fraction(int num, int den) : numerator(num), denominator(den) {
    if (den == 0) {
        throw std::invalid_argument("Denominator cannot be zero!");
    }
	if (den < 0) {
		numerator = -numerator;
		denominator = -denominator;
	}
}