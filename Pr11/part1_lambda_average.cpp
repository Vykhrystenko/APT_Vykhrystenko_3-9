#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <optional>
#include <vector>
#include <windows.h>

std::optional<double> calculateAverage(
    const std::vector<int>& numbers)
{
    if (numbers.empty()) {
        return std::nullopt;
    }

    const double sum = std::accumulate(
        numbers.begin(),
        numbers.end(),
        0.0,
        [](double total, int value) {
            return total + value;
        }
    );

    return sum / static_cast<double>(numbers.size());
}

void printVector(const std::vector<int>& numbers) {
    std::for_each(
        numbers.begin(),
        numbers.end(),
        [](int value) {
            std::cout << value << ' ';
        }
    );

    std::cout << '\n';
}

int main() {
    SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
    const std::vector<int> numbers{ 10, 20, 30, 40, 50 };

    std::cout << "Вектор: ";
    printVector(numbers);

    const auto average = calculateAverage(numbers);

    if (average.has_value()) {
        std::cout << "Середнє арифметичне: "
            << std::fixed << std::setprecision(2)
            << average.value() << '\n';
    }
    else {
        std::cout << "Середнє арифметичне не існує: "
            "вектор порожній.\n";
    }

    const std::vector<int> empty;

    const auto emptyAverage = calculateAverage(empty);

    std::cout << "Перевірка порожнього вектора: ";

    if (emptyAverage) {
        std::cout << *emptyAverage << '\n';
    }
    else {
        std::cout << "std::nullopt\n";
    }

    return 0;
}