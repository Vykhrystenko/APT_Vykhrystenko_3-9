#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>
#include "../include/dtofraction.h"
#include "../include/fractcomparison.h"

// Тести базового алгоритму наближення (DtoFraction)
TEST(DtoFractionTest, PiApproximationStandardPrecision) {
    double v = 3.1415926535;
    double error = 1.0e-10;

    Fraction result = DtoFraction::fraction(v, error);

    EXPECT_EQ(result.getNumerator(), 104348);
    EXPECT_EQ(result.getDenominator(), 33215);

    double rawError = std::fabs(1.0 - static_cast<double>(result.getNumerator()) / (v * result.getDenominator()));
    double m = 1.0;
    while (m * rawError < 1.0) {
        m *= 10.0;
    }
    double roundedEpsilon = (1.0 / m) * static_cast<int>(0.5 + m * rawError);
    EXPECT_LE(roundedEpsilon, error);
}

TEST(DtoFractionTest, PiApproximationHigherPrecisionReachesFifthStep) {
    double v = 3.1415926535;
    double error = 1.0e-11;

    Fraction result = DtoFraction::fraction(v, error);

    EXPECT_EQ(result.getNumerator(), 208341);
    EXPECT_EQ(result.getDenominator(), 66317);

    double rawError = std::fabs(1.0 - static_cast<double>(result.getNumerator()) / (v * result.getDenominator()));
    EXPECT_NEAR(rawError, 1.036518e-11, 1.0e-15);
}

TEST(DtoFractionTest, NegativeNumberApproximation) {
    double v = -3.1415926535;
    double error = 1.0e-3;

    Fraction result = DtoFraction::fraction(v, error);

    EXPECT_EQ(result.getNumerator(), -22);
    EXPECT_EQ(result.getDenominator(), 7);
}

TEST(DtoFractionTest, ExactIntegerValue) {
    Fraction result = DtoFraction::fraction(5.0, 1.0e-6);

    EXPECT_EQ(result.getNumerator(), 5);
    EXPECT_EQ(result.getDenominator(), 1);
}

TEST(DtoFractionTest, ZeroValue) {
    Fraction result = DtoFraction::fraction(0.0, 1.0e-6);

    EXPECT_EQ(result.getNumerator(), 0);
    EXPECT_EQ(result.getDenominator(), 1);
}

TEST(DtoFractionTest, ThrowsOnNegativeErrorTolerance) {
    EXPECT_THROW(DtoFraction::fraction(3.14, -0.01), std::invalid_argument);
}

// Тести класу Fraction та індивідуального розширення (FractComparison)

TEST(FractionTest, ThrowsOnZeroDenominator) {
    EXPECT_THROW(Fraction(1, 0), std::invalid_argument);
}

TEST(FractComparisonTest, EqualsComparison) {
    Fraction f1(1, 2);
    Fraction f2(2, 4);
    Fraction f3(3, 5);

    EXPECT_TRUE(FractComparison::equals(f1, f2));
    EXPECT_FALSE(FractComparison::equals(f1, f3));
}

TEST(FractComparisonTest, LessAndGreaterComparison) {
    Fraction small(1, 9);
    Fraction large(4, 1);

    EXPECT_TRUE(FractComparison::lessThan(small, large));
    EXPECT_FALSE(FractComparison::greaterThan(small, large));

    EXPECT_TRUE(FractComparison::greaterThan(large, small));
    EXPECT_FALSE(FractComparison::lessThan(large, small));
}