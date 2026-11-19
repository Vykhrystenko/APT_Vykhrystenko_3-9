#include "../include/DtoFraction.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <utility>

void DtoFraction::validateInput(double v, double error) {
    if (v < std::numeric_limits<double>::lowest() ||
        v > std::numeric_limits<double>::max() ||
        error < 0.0) {
        throw std::invalid_argument("Invalid input value or negative error tolerance.");
    }
}

void DtoFraction::updateMediants(int& n, int& d, int& N, int& D, double& r) {
    if (r <= 1.0) {
        r = 1.0 / r;
    }
    N += n * static_cast<int>(r);
    D += d * static_cast<int>(r);
    n += N;
    d += D;
}

void DtoFraction::refineBounds(double v, int& n, int& d, int& N, int& D, double& r) {
    r = 0.0;
    if (v * d != static_cast<double>(n)) {
        r = (N - v * D) / (v * d - n);
        if (r <= 1.0) {
            std::swap(N, n);
            std::swap(D, d);
        }
    }
}

double DtoFraction::calculateRoundedEpsilon(double v, int n, int d, double error) {
    if (v == 0.0) {
        return 0.0;
    }

    double epsilon = std::fabs(1.0 - static_cast<double>(n) / (v * d));
    if (epsilon <= error) {
        return epsilon;
    }

    double m = 1.0;
    do {
        m *= 10.0;
    } while (m * epsilon < 1.0);

    return (1.0 / m) * static_cast<int>(0.5 + m * epsilon);
}

void DtoFraction::printStep(int sign, int n, int d, double epsilon) {
    std::cout << (sign * n) << "/" << d
        << "\tepsilon = " << std::scientific << std::setprecision(6) << epsilon
        << std::defaultfloat << std::endl;
}

Fraction DtoFraction::fraction(double v, double error) {
    validateInput(v, error);

    int sign = (v < 0.0) ? -1 : 1;
    v = std::fabs(v);

    int d = 1;
    int D = 1;
    int n = static_cast<int>(v);
    int N = n + 1;
    double r = 0.0;

    bool isFirstStep = true;

    while (true) {
        if (!isFirstStep) {
            updateMediants(n, d, N, D, r);
        }
        isFirstStep = false;

        refineBounds(v, n, d, N, D, r);

        double epsilon = calculateRoundedEpsilon(v, n, d, error);
        printStep(sign, n, d, epsilon);

        if (epsilon <= error || r == 0.0) {
            break;
        }
    }

    return Fraction(sign * n, d);
}