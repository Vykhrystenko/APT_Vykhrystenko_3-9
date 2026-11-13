#include <iostream>
#include <exception>

#include "MatrixLib.h"
#include "MatrixExceptions.h"
#include "ScopeLogger.h"

// Виведення матриці
void printMatrix(const int* matrix, int rows, int cols)
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i * cols + j] << " ";
        }

        std::cout << '\n';
    }
}

// Рівень 3: виникнення винятку
void level3()
{
    ScopeLogger logger("level3");

    int A[] = { 1, 2, 3, 4 };
    int B[] = { 1, 2, 3, 4, 5, 6 };
    int result[4] = {};

    std::cout << "Trying to add incompatible matrices...\n";

    MatrixLib::add(
        A, 2, 2,
        B, 2, 3,
        result);
}

// Рівень 2: перехоплення та повторне генерування
void level2()
{
    ScopeLogger logger("level2");

    try {
        level3();
    }
    catch (const std::exception& ex) {
        std::cout << "[LOG] level2 caught: "
            << ex.what() << '\n';

        throw;
    }
}

// Рівень 1: виклик наступного рівня
void level1()
{
    ScopeLogger logger("level1");

    level2();
}

int main()
{
    ScopeLogger mainLogger("main");

    try {
        // демонстрація роботи статичної бібліотеки
        const int A[] = {
            1, 2,
            3, 4
        };

        const int B[] = {
            5, 6,
            7, 8
        };

        int result[4] = {};

        std::cout << "=== Matrix addition ===\n";

        MatrixLib::add(A, 2, 2, B, 2, 2, result);
        printMatrix(result, 2, 2);

        std::cout << "\n=== Matrix multiplication ===\n";

        MatrixLib::multiply(A, 2, 2, B, 2, 2, result);
        printMatrix(result, 2, 2);

        std::cout << "\n=== Matrix transpose ===\n";

        MatrixLib::transpose(A, 2, 2, result);
        printMatrix(result, 2, 2);

        std::cout << "\n=== Matrix-vector multiplication ===\n";

        const int v[] = { 2, 3 };
        int vectorResult[2] = {};

        MatrixLib::multiplyVector(
            A, 2, 2,
            v, 2,
            vectorResult);

        for (int value : vectorResult) {
            std::cout << value << " ";
        }

        //Демонстрація винятків
        std::cout << "\n\n=== Exception demonstration ===\n";

        level1();
    }
    catch (const MatrixDimensionException& ex) {
        std::cout << "[MAIN] Dimension error: "
            << ex.what() << '\n';
    }
    catch (const MatrixException& ex) {
        std::cout << "[MAIN] Matrix error: "
            << ex.what() << '\n';
    }
    catch (const std::exception& ex) {
        std::cout << "[MAIN] Standard error: "
            << ex.what() << '\n';
    }

    std::cout << "Program continues normally.\n";

    return 0;
}