#include <iostream>
#include "LinearSystems.h"

int main() {
    // Система:
    // 2*x1 + x2 = 5
    // x1 - x2 = 1
    const double A[] = {
        2.0,  1.0,
        1.0, -1.0
    };

    const double b[] = { 5.0, 1.0 };
    double x[2]{};

    if (!solve(A, b, 2, x)) {
        std::cerr << "Error: Failed to solve the system.\n";
        return 1;
    }

    const double r = residual(nullptr, b, x, 2);
    const int valid = checkSolution(A, nullptr, x, 2, 1e-9);

    std::cout << "--- Implicit Linking ---" << std::endl;
    std::cout << "x1 = " << x[0] << '\n';
    std::cout << "x2 = " << x[1] << '\n';
    std::cout << "Residual = " << r << '\n';
    std::cout << "Checked = " << valid << '\n';

    return 0;
}