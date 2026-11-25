#include <algorithm>
#include <iostream>
#include <vector>
#include <windows.h>

std::vector<int> prependElement(
    const std::vector<int>& source,
    int element)
{
    std::vector<int> result;

    result.reserve(source.size() + 1);

    result.push_back(element);

    result.insert(
        result.end(),
        source.begin(),
        source.end()
    );

    return result;
}

void printVector(const std::vector<int>& numbers) {
    std::cout << '{';

    bool first = true;

    std::for_each(
        numbers.begin(),
        numbers.end(),
        [&first](int value) {
            if (!first) {
                std::cout << ", ";
            }

            std::cout << value;
            first = false;
        }
    );

    std::cout << "}\n";
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    const std::vector<int> numbers{ 2, 4, 6, 8 };

    const int element = 1;

    std::cout << "Початковий вектор до виклику: ";
    printVector(numbers);

    const std::vector<int> result =
        prependElement(numbers, element);

    std::cout << "Новий вектор: ";
    printVector(result);

    std::cout << "Початковий вектор після виклику: ";
    printVector(numbers);

    const std::vector<int> empty;

    const auto resultFromEmpty =
        prependElement(empty, 99);

    std::cout << "Результат для порожнього вектора: ";
    printVector(resultFromEmpty);

    return 0;
}