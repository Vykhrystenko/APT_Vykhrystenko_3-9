#include "include/DtoFraction.h"
#include <iostream>

int main() {
    double error = 1.0e-10;
    double v = 3.1415926535;

    Fraction a1 = DtoFraction::fraction(v, error);

    return 0;
}