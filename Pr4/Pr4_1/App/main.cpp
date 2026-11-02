#include <iostream>
#include "MatrixLib.h" 

int main() {

    // Створюємо тестові матриці 2x2 у вигляді лінійних масивів
    // Matrix A:
    // [ 1, 2 ]
    // [ 3, 4 ]
    const int rowsA = 2;
    const int colsA = 2;
    int A[rowsA * colsA] = { 1, 2,
                           3, 4 };

    // Matrix B:
    // [ 5, 6 ]
    // [ 7, 8 ]
    const int rowsB = 2;
    const int colsB = 2;
    int B[rowsB * colsB] = { 5, 6,
                           7, 8 };

    // 1. Тест функції isCompatible
    std::cout << "\n1. Checking matrix compatibility (isCompatible):" << std::endl;
    bool compatible = MatrixLib::isCompatible(rowsA, colsA, rowsB, colsB);
    std::cout << "Are matrices A and B compatible for addition? " << (compatible ? "Yes" : "No") << std::endl;

    // 2. Тест функції addMatrices (Додавання)
    std::cout << "\n2. Matrix addition (addMatrices):" << std::endl;
    int resultAdd[rowsA * colsA] = { 0 };
    if (MatrixLib::add(A, rowsA, colsA, B, rowsB, colsB, resultAdd)) {
        std::cout << "Result (A + B):" << std::endl;
        for (int i = 0; i < rowsA; ++i) {
            for (int j = 0; j < colsA; ++j) {
                std::cout << resultAdd[i * colsA + j] << " ";
            }
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "Error during matrix addition!" << std::endl;
    }

    // 3. Тест функції multiplyMatrices (Множення матриць)
    std::cout << "\n3. Matrix multiplication (multiplyMatrices):" << std::endl;
    int resultMultiply[rowsA * colsB] = { 0 };
    if (MatrixLib::multiply(A, rowsA, colsA, B, rowsB, colsB, resultMultiply)) {
        std::cout << "Result (A * B):" << std::endl;
        for (int i = 0; i < rowsA; ++i) {
            for (int j = 0; j < colsB; ++j) {
                std::cout << resultMultiply[i * colsB + j] << " ";
            }
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "Error during matrix multiplication!" << std::endl;
    }

    // 4. Тест функції transposeMatrix (Транспонування)
    std::cout << "\n4. Transpose matrix A (transposeMatrix):" << std::endl;
    int resultTranspose[rowsA * colsA] = { 0 };
    if (MatrixLib::transpose(A, rowsA, colsA, resultTranspose)) {
        std::cout << "Result (transposed A):" << std::endl;
        for (int i = 0; i < colsA; ++i) { // Розміри міняються місцями
            for (int j = 0; j < rowsA; ++j) {
                std::cout << resultTranspose[i * rowsA + j] << " ";
            }
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "Error during matrix transposition!" << std::endl;
    }

    // 5. Тест функції multiplyVector (Множення матриці на вектор)
    std::cout << "\n5. Matrix-vector multiplication (multiplyVector):" << std::endl;
    int vSize = 2;
    int v[2] = { 1, 2 };
    int resultVec[2] = { 0 };

    if (MatrixLib::multiplyVector(A, rowsA, colsA, v, vSize, resultVec)) {
        std::cout << "Result (A * v): ";
        for (int i = 0; i < rowsA; ++i) {
            std::cout << resultVec[i] << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "Error during matrix-vector multiplication!" << std::endl;
    }

    return 0;
}