#pragma once

class Fraction{
private:
    int numerator;
    int denominator;
public:
    Fraction(int num = 0, int den = 1);
    ~Fraction() = default;
    int getNumerator() const { return numerator; }
    int getDenominator() const { return denominator; }
};