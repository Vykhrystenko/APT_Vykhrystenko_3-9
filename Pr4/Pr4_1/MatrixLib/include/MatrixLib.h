#pragma once

#define MATRIXLIB_API 

namespace MatrixLib {
	// Функція для перевірки сумісності двох матриць для операцій додавання та множення
    MATRIXLIB_API bool isCompatible(int rowsA, int colsA, int rowsB, int colsB);
	// Функція для додавання двох матриць
    MATRIXLIB_API bool add(
        const int* A, int rowsA, int colsA,
        const int* B, int rowsB, int colsB,
        int* result);
	// Функція для множення двох матриць
    MATRIXLIB_API bool multiply(
        const int* A, int rowsA, int colsA,
        const int* B, int rowsB, int colsB,
        int* result);
	// Функція для транспонування матриці
    MATRIXLIB_API bool transpose(
        const int* A, int rows, int cols,
        int* result);
	// Функція для множення матриці на вектор
    MATRIXLIB_API bool multiplyVector(
        const int* A, int rows, int cols,
        const int* v, int vSize,
        int* result);
}