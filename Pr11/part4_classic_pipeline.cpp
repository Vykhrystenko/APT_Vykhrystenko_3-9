#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <vector>
#include <windows.h>

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

    const std::vector<int> numbers{
        1, 2, 3, 4, 5, 6, 7, 8
    };

    std::vector<int> oddNumbers;

    std::copy_if(
        numbers.begin(),
        numbers.end(),
        std::back_inserter(oddNumbers),
        [](int value) {
            return value % 2 != 0;
        }
    );

    std::vector<int> squaredOddNumbers(oddNumbers.size());

    std::transform(
        oddNumbers.begin(),
        oddNumbers.end(),
        squaredOddNumbers.begin(),
        [](int value) {
            return value * value;
        }
    );

    std::optional<double> average;

    if (!squaredOddNumbers.empty()) {
        const double sum = std::accumulate(
            squaredOddNumbers.begin(),
            squaredOddNumbers.end(),
            0.0,
            [](double total, int value) {
                return total + value;
            }
        );

        average = sum /
            static_cast<double>(squaredOddNumbers.size());
    }

    std::cout << "Початковий вектор: ";
    printVector(numbers);

    std::cout << "Відібрані непарні числа: ";
    printVector(oddNumbers);

    std::cout << "Квадрати непарних чисел: ";
    printVector(squaredOddNumbers);

    if (average) {
        std::cout << "Середнє арифметичне квадратів: "
            << std::fixed << std::setprecision(2)
            << *average << '\n';
    }
    else {
        std::cout << "Середнє не існує: "
            "після фільтрації немає елементів.\n";
    }

    return 0;
}