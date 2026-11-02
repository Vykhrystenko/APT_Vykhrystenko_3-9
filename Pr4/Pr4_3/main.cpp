#include <iostream>
#include <windows.h>

int main() {
    // Завантаження динамічної бібліотеки партнера
    HMODULE hLib = LoadLibrary(TEXT("LinearSystems.dll"));
    if (!hLib) {
        std::cout << "Помилка: не вдалося знайти LinearSystems.dll!\n";
        return 1;
    }

    // Отримання вказівників на функції
    auto solve = (int(*)(const double*, const double*, int, double*))GetProcAddress(hLib, "solve");
    auto residual = (double(*)(const double*, const double*, const double*, int))GetProcAddress(hLib, "residual");
    auto checkSolution = (int(*)(const double*, const double*, const double*, int, double))GetProcAddress(hLib, "checkSolution");

    // Підготовка тестових даних (СЛАР)
    int n = 2;
    double A[] = {
        2.0,  1.0,
        1.0, -1.0
    };
    double b[] = { 5.0, 1.0 };
    double x[2] = { 0.0, 0.0 };

    // Виклик функцій бібліотеки
    solve(A, b, n, x);
    double r = residual(A, b, x, n);
    int valid = checkSolution(A, b, x, n, 1e-9);

    // Виведення результатів
    std::cout << "x1 = " << x[0] << "\n";
    std::cout << "x2 = " << x[1] << "\n";
    std::cout << "Residual = " << r << "\n";
    std::cout << "Checked = " << valid << "\n";

    // Вивантаження бібліотеки
    FreeLibrary(hLib);
    return 0;
}