#include "MatrixLib.h"
#include <stdexcept>
using namespace std;

namespace MatrixLib {
        bool isCompatible(int rowsA, int colsA, int rowsB, int colsB) {
            if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0) {
                return false;
            }
            return (rowsA == rowsB && colsA == colsB);
        }

        // Додавання двох матриць 
        bool add(
            const int* A, int rowsA, int colsA,
            const int* B, int rowsB, int colsB,
            int* result)
        {
            if (!isCompatible(rowsA, colsA, rowsB, colsB) || !A || !B || !result) {
                return false;
            }

            int totalElements = rowsA * colsA;
            for (int i = 0; i < totalElements; ++i) {
                result[i] = A[i] + B[i];
            }
            return true;
        }

        // Множення двох матриць
        bool multiply(
            const int* A, int rowsA, int colsA,
            const int* B, int rowsB, int colsB,
            int* result)
        {
            if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0 || !A || !B || !result) {
                return false;
            }

            if (colsA != rowsB) {
                return false; 
            }

            // Ініціалізація результуючої матриці нулями
            for (int i = 0; i < rowsA * colsB; ++i) {
                result[i] = 0;
            }

            // Алгоритм множення матриць
            for (int i = 0; i < rowsA; ++i) {
                for (int j = 0; j < colsB; ++j) {
                    for (int k = 0; k < colsA; ++k) {
                        result[i * colsB + j] += A[i * colsA + k] * B[k * colsB + j];
                    }
                }
            }
            return true;
        }

        // Транспонування матриці
        bool transpose(
            const int* A, int rows, int cols,
            int* result)
        {
            if (rows <= 0 || cols <= 0 || !A || !result) {
                return false;
            }

            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    result[j * rows + i] = A[i * cols + j];
                }
            }
            return true;
        }

        // Множення матриці на вектор
        bool multiplyVector(
            const int* A, int rows, int cols,
            const int* v, int vSize,
            int* result)
        {
            if (rows <= 0 || cols <= 0 || vSize <= 0 || !A || !v || !result) {
                return false;
            }

            if (vSize != cols) {
                return false; 
            }

            for (int i = 0; i < rows; ++i) {
                result[i] = 0;
                for (int j = 0; j < cols; ++j) {
                    result[i] += A[i * cols + j] * v[j];
                }
            }
            return true;
        }
    }