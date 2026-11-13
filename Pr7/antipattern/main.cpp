#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <limits>

// Антипатерн 1: виняток типу int
void throwInteger()
{
    throw 42;
}

// Антипатерн 2: виняток типу std::string
void throwString()
{
    throw std::string("Invalid matrix dimensions");
}

// Рекомендований підхід
void throwStandardException()
{
    throw std::invalid_argument("Invalid matrix dimensions");
}

// Демонстрація object slicing
void demonstrateSlicing()
{
    try {
        throw std::invalid_argument("Invalid argument");
    }
    catch (const std::exception& ex) {

        // Неправильно: можлива втрата похідного типу
        throw ex;
    }
}

// Демонстрація порушення сильної гарантії
void unsafeAdd(
    const int* A,
    const int* B,
    int* result,
    int count)
{
    for (int i = 0; i < count; ++i) {

        // Обчислюємо та одразу записуємо результат
        const long long sum =
            static_cast<long long>(A[i]) + B[i];

        if (sum < std::numeric_limits<int>::min() ||
            sum > std::numeric_limits<int>::max())
        {
            throw std::overflow_error("Integer overflow");
        }

        result[i] = static_cast<int>(sum);
    }
}

int main()
{
    std::cout << "=== Example 1: int exception ===\n";

    try {
        throwInteger();
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << '\n';
    }
    catch (int error) {
        std::cout << "Caught int: " << error << '\n';
    }

    std::cout << "\n=== Example 2: string exception ===\n";

    try {
        throwString();
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << '\n';
    }
    catch (const std::string& error) {
        std::cout << "Caught string: " << error << '\n';
    }

    std::cout << "\n=== Example 3: standard exception ===\n";

    try {
        throwStandardException();
    }
    catch (const std::exception& ex) {
        std::cout << "Caught standard exception: "
            << ex.what() << '\n';
    }

    std::cout << "\n=== Example 4: object slicing ===\n";

    try {
        demonstrateSlicing();
    }
    catch (const std::invalid_argument& ex) {
        std::cout << "Caught invalid_argument\n";
    }
    catch (const std::exception& ex) {
        std::cout << "Caught base std::exception: "
            << ex.what() << '\n';
    }

    std::cout << "\n=== Example 5: unsafe addition ===\n";

    const int A[] = {
        10,
        std::numeric_limits<int>::max()
    };

    const int B[] = { 1, 1 };

    int result[] = { -1, -2 };

    try {
        unsafeAdd(A, B, result, 2);
    }
    catch (const std::exception& ex) {
        std::cout << "Exception: " << ex.what() << '\n';
    }

    std::cout << "Result after failed operation: ";

    for (int value : result) {
        std::cout << value << " ";
    }

    std::cout << '\n';

    return 0;
}