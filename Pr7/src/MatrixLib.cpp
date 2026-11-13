#include "MatrixLib.h"
#include "MatrixExceptions.h"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <string>
#include <vector>
#include <stdexcept>


namespace {

    // Перевірка розмірів матриці
    void validateDimensions(
        const char* parameter,
        int rows,
        int cols)
    {
        if (rows <= 0 || cols <= 0) {
            throw InvalidMatrixException(
                parameter,
                std::string(parameter) +
                ": dimensions must be positive");
        }
    }

    // Перевірка вказівника
    void validatePointer(
        const int* pointer,
        const char* parameter)
    {
        if (pointer == nullptr) {
            throw InvalidMatrixException(
                parameter,
                std::string(parameter) +
                ": pointer cannot be null");
        }
    }

    // Безпечне обчислення кількості елементів
    std::size_t elementCount(int rows, int cols)
    {
        const std::size_t r = static_cast<std::size_t>(rows);
        const std::size_t c = static_cast<std::size_t>(cols);

        if (r > std::numeric_limits<std::size_t>::max() / c) {
            throw std::overflow_error("Matrix size overflow");
        }

        return r * c;
    }

    // Перевірка, чи результат поміщається в int
    int checkedInt(long long value)
    {
        if (value < std::numeric_limits<int>::min() ||
            value > std::numeric_limits<int>::max())
        {
            throw std::overflow_error(
                "Matrix operation result exceeds int range");
        }

        return static_cast<int>(value);
    }

    // Безпечне додавання двох long long
    long long checkedLongLongAdd(long long a, long long b)
    {
        if (b > 0 &&
            a > std::numeric_limits<long long>::max() - b)
        {
            throw std::overflow_error("Arithmetic overflow");
        }

        if (b < 0 &&
            a < std::numeric_limits<long long>::min() - b)
        {
            throw std::overflow_error("Arithmetic overflow");
        }

        return a + b;
    }
}

namespace MatrixLib {

    // Перевірка сумісності розмірів
    bool isCompatible(
        int rowsA, int colsA,
        int rowsB, int colsB) noexcept
    {
        return rowsA > 0 &&
            colsA > 0 &&
            rowsB > 0 &&
            colsB > 0 &&
            rowsA == rowsB &&
            colsA == colsB;
    }

    // Додавання двох матриць
    void add(
        const int* A, int rowsA, int colsA,
        const int* B, int rowsB, int colsB,
        int* result)
    {
        validateDimensions("A", rowsA, colsA);
        validateDimensions("B", rowsB, colsB);

        validatePointer(A, "A");
        validatePointer(B, "B");
        validatePointer(result, "result");

        if (!isCompatible(rowsA, colsA, rowsB, colsB)) {
            throw MatrixDimensionException(
                "add",
                "Matrices must have the same dimensions");
        }

        const std::size_t count =
            elementCount(rowsA, colsA);

        // Тимчасове збереження результату
        std::vector<int> temp(count);

        for (std::size_t i = 0; i < count; ++i) {
            const long long sum =
                static_cast<long long>(A[i]) + B[i];

            temp[i] = checkedInt(sum);
        }

        // Фіксація результату після успішних обчислень
        std::copy(temp.begin(), temp.end(), result);
    }

    // Множення двох матриць
    void multiply(
        const int* A, int rowsA, int colsA,
        const int* B, int rowsB, int colsB,
        int* result)
    {
        validateDimensions("A", rowsA, colsA);
        validateDimensions("B", rowsB, colsB);

        validatePointer(A, "A");
        validatePointer(B, "B");
        validatePointer(result, "result");

        if (colsA != rowsB) {
            throw MatrixDimensionException(
                "multiply",
                "Columns of A must equal rows of B");
        }

        const std::size_t count =
            elementCount(rowsA, colsB);

        std::vector<int> temp(count, 0);

        for (int i = 0; i < rowsA; ++i) {
            for (int j = 0; j < colsB; ++j) {

                long long sum = 0;

                for (int k = 0; k < colsA; ++k) {

                    const long long product =
                        static_cast<long long>(
                            A[static_cast<std::size_t>(i) * colsA + k])
                        *
                        B[static_cast<std::size_t>(k) * colsB + j];

                    sum = checkedLongLongAdd(sum, product);
                }

                temp[static_cast<std::size_t>(i) * colsB + j] =
                    checkedInt(sum);
            }
        }

        std::copy(temp.begin(), temp.end(), result);
    }

    // Транспонування матриці
    void transpose(
        const int* A, int rows, int cols,
        int* result)
    {
        validateDimensions("A", rows, cols);

        validatePointer(A, "A");
        validatePointer(result, "result");

        const std::size_t count =
            elementCount(rows, cols);

        std::vector<int> temp(count);

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {

                temp[static_cast<std::size_t>(j) * rows + i] =
                    A[static_cast<std::size_t>(i) * cols + j];
            }
        }

        std::copy(temp.begin(), temp.end(), result);
    }

    // Множення матриці на вектор
    void multiplyVector(
        const int* A, int rows, int cols,
        const int* v, int vSize,
        int* result)
    {
        validateDimensions("A", rows, cols);

        if (vSize <= 0) {
            throw InvalidMatrixException(
                "vSize",
                "Vector size must be positive");
        }

        validatePointer(A, "A");
        validatePointer(v, "v");
        validatePointer(result, "result");

        if (vSize != cols) {
            throw MatrixDimensionException(
                "multiplyVector",
                "Vector size must equal matrix columns");
        }

        std::vector<int> temp(
            static_cast<std::size_t>(rows));

        for (int i = 0; i < rows; ++i) {

            long long sum = 0;

            for (int j = 0; j < cols; ++j) {

                const long long product =
                    static_cast<long long>(
                        A[static_cast<std::size_t>(i) * cols + j])
                    * v[j];

                sum = checkedLongLongAdd(sum, product);
            }

            temp[i] = checkedInt(sum);
        }

        std::copy(temp.begin(), temp.end(), result);
    }
}