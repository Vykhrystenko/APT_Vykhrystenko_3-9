#include <gtest/gtest.h>

#include "MatrixLib.h"
#include "MatrixExceptions.h"

#include <limits>
#include <stdexcept>

// 1. Перевірка додавання матриць
TEST(MatrixLibTest, AddWorks)
{
    const int A[] = {
        1, 2,
        3, 4
    };

    const int B[] = {
        5, 6,
        7, 8
    };

    int result[4] = {};

    MatrixLib::add(A, 2, 2, B, 2, 2, result);

    const int expected[] = { 6, 8, 10, 12 };

    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(result[i], expected[i]);
    }
}

// 2. Перевірка несумісних розмірів під час додавання
TEST(MatrixLibTest, AddRejectsDifferentDimensions)
{
    const int A[] = { 1, 2, 3, 4 };
    const int B[] = { 1, 2, 3, 4, 5, 6 };
    int result[6] = {};

    EXPECT_THROW(
        MatrixLib::add(A, 2, 2, B, 2, 3, result),
        MatrixDimensionException
    );
}

// 3. Перевірка нульового вказівника
TEST(MatrixLibTest, AddRejectsNullPointer)
{
    const int B[] = { 1 };
    int result[1] = {};

    EXPECT_THROW(
        MatrixLib::add(nullptr, 1, 1, B, 1, 1, result),
        InvalidMatrixException
    );
}

// 4. Перевірка сильної гарантії під час додавання
TEST(MatrixLibTest, AddPreservesResultOnOverflow)
{
    const int A[] = {
        1,
        std::numeric_limits<int>::max()
    };

    const int B[] = {
        2,
        1
    };

    int result[] = { -1, -2 };

    EXPECT_THROW(
        MatrixLib::add(A, 1, 2, B, 1, 2, result),
        std::overflow_error
    );

    EXPECT_EQ(result[0], -1);
    EXPECT_EQ(result[1], -2);
}

// 5. Перевірка множення матриць
TEST(MatrixLibTest, MultiplyWorks)
{
    const int A[] = {
        1, 2, 3,
        4, 5, 6
    };

    const int B[] = {
        7, 8,
        9, 10,
        11, 12
    };

    int result[4] = {};

    MatrixLib::multiply(A, 2, 3, B, 3, 2, result);

    const int expected[] = { 58, 64, 139, 154 };

    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(result[i], expected[i]);
    }
}

// 6. Перевірка неправильних розмірів під час множення
TEST(MatrixLibTest, MultiplyRejectsIncompatibleDimensions)
{
    const int A[] = { 1, 2, 3, 4, 5, 6 };
    const int B[] = { 1, 2, 3, 4 };
    int result[4] = {};

    EXPECT_THROW(
        MatrixLib::multiply(A, 2, 3, B, 2, 2, result),
        MatrixDimensionException
    );
}

// 7. Перевірка сильної гарантії під час множення
TEST(MatrixLibTest, MultiplyPreservesResultOnOverflow)
{
    const int A[] = {
        std::numeric_limits<int>::max()
    };

    const int B[] = { 2 };

    int result[] = { -10 };

    EXPECT_THROW(
        MatrixLib::multiply(A, 1, 1, B, 1, 1, result),
        std::overflow_error
    );

    EXPECT_EQ(result[0], -10);
}

// 8. Перевірка транспонування
TEST(MatrixLibTest, TransposeWorks)
{
    const int A[] = {
        1, 2, 3,
        4, 5, 6
    };

    int result[6] = {};

    MatrixLib::transpose(A, 2, 3, result);

    const int expected[] = {
        1, 4,
        2, 5,
        3, 6
    };

    for (int i = 0; i < 6; ++i) {
        EXPECT_EQ(result[i], expected[i]);
    }
}

// 9. Перевірка множення матриці на вектор
TEST(MatrixLibTest, MultiplyVectorWorks)
{
    const int A[] = {
        1, 2, 3,
        4, 5, 6
    };

    const int v[] = { 1, 2, 3 };

    int result[2] = {};

    MatrixLib::multiplyVector(A, 2, 3, v, 3, result);

    EXPECT_EQ(result[0], 14);
    EXPECT_EQ(result[1], 32);
}

// 10. Перевірка неправильного розміру вектора
TEST(MatrixLibTest, MultiplyVectorRejectsWrongSize)
{
    const int A[] = { 1, 2, 3, 4, 5, 6 };
    const int v[] = { 1, 2 };
    int result[2] = {};

    EXPECT_THROW(
        MatrixLib::multiplyVector(A, 2, 3, v, 2, result),
        MatrixDimensionException
    );
}

// 11. Перевірка недопустимих розмірів
TEST(MatrixLibTest, RejectsNonPositiveDimensions)
{
    const int A[] = { 1 };
    int result[1] = {};

    EXPECT_THROW(
        MatrixLib::transpose(A, 0, 1, result),
        InvalidMatrixException
    );
}

// 12. Перевірка перехоплення через базовий клас
TEST(MatrixLibTest, ExceptionIsCatchableThroughBase)
{
    const int A[] = {1};
    const int B[] = {2,3};
    int result[1] = {};

    bool caught = false;

    try {
        MatrixLib::add(A, 1, 1, B, 1, 2, result);
    }
    catch (const MatrixException&) {
        caught = true;
    }

    EXPECT_TRUE(caught);
}

// 13. Перевірка допоміжної функції сумісності
TEST(MatrixLibTest, CompatibilityCheckWorks)
{
    EXPECT_TRUE(MatrixLib::isCompatible(2, 3, 2, 3));

    EXPECT_FALSE(MatrixLib::isCompatible(2, 3, 3, 2));

    EXPECT_FALSE(MatrixLib::isCompatible(0, 3, 0, 3));
}