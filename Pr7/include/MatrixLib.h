#pragma once

namespace MatrixLib {

    // Перевірка сумісності розмірів матриць
    bool isCompatible(
        int rowsA, int colsA,
        int rowsB, int colsB) noexcept;

    // Додавання двох матриць
    void add(
        const int* A, int rowsA, int colsA,
        const int* B, int rowsB, int colsB,
        int* result);

    // Множення двох матриць
    void multiply(
        const int* A, int rowsA, int colsA,
        const int* B, int rowsB, int colsB,
        int* result);

    // Транспонування матриці
    void transpose(
        const int* A, int rows, int cols,
        int* result);

    // Множення матриці на вектор
    void multiplyVector(
        const int* A, int rows, int cols,
        const int* v, int vSize,
        int* result);
}