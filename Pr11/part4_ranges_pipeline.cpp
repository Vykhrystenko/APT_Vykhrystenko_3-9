#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <utility>
#include <vector>
#include <windows.h>

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    const std::vector<int> numbers{
        1, 2, 3, 4, 5, 6, 7, 8
    };

    auto oddNumbers = numbers
        | std::views::filter([](int value) {
        return value % 2 != 0;
            });

    auto squaredOddNumbers = oddNumbers
        | std::views::transform([](int value) {
        return value * value;
            });

    std::cout << "Початковий вектор: ";

    std::ranges::for_each(numbers, [](int value) {
        std::cout << value << ' ';
        });

    std::cout << '\n';

    std::cout << "Відібрані непарні числа: ";

    std::ranges::for_each(oddNumbers, [](int value) {
        std::cout << value << ' ';
        });

    std::cout << '\n';

    std::cout << "Квадрати непарних чисел: ";

    std::ranges::for_each(squaredOddNumbers, [](int value) {
        std::cout << value << ' ';
        });

    std::cout << '\n';

    const auto sumAndCount = std::accumulate(
        squaredOddNumbers.begin(),
        squaredOddNumbers.end(),
        std::pair<double, std::size_t>{0.0, 0},
        [](std::pair<double, std::size_t> state, int value) {
            state.first += static_cast<double>(value);
            ++state.second;

            return state;
        }
    );

    const std::optional<double> average =
        sumAndCount.second == 0
        ? std::nullopt
        : std::optional<double>{
            sumAndCount.first /
            static_cast<double>(sumAndCount.second)
    };

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