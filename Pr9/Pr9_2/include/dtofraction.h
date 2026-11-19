#pragma once
#include "fraction.h"

class DtoFraction {
private:
    static void validateInput(double v, double error);
    static void updateMediants(int& n, int& d, int& N, int& D, double& r);
    static void refineBounds(double v, int& n, int& d, int& N, int& D, double& r);
    static double calculateRoundedEpsilon(double v, int n, int d, double error);
    static void printStep(int sign, int n, int d, double epsilon);

public:
    static Fraction fraction(double v, double error);
};