#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
#include <windows.h>

class GreaterThan {
private:
    int threshold;

public:
    explicit GreaterThan(int value)
        : threshold(value) {
    }

    bool operator()(int number) const {
        return number > threshold;
    }
};

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
        3, 8, 12, 5, 17, 9, 20
    };

    const int threshold = 10;

    std::vector<int> functorResult;

    std::copy_if(
        numbers.begin(),
        numbers.end(),
        std::back_inserter(functorResult),
        GreaterThan(threshold)
    );

    std::vector<int> lambdaResult;

    std::copy_if(
        numbers.begin(),
        numbers.end(),
        std::back_inserter(lambdaResult),
        [threshold](int number) {
            return number > threshold;
        }
    );

    std::cout << "Початковий вектор: ";
    printVector(numbers);

    std::cout << "Результат з функтором GreaterThan: ";
    printVector(functorResult);

    std::cout << "Результат з лямбда-виразом: ";
    printVector(lambdaResult);

    std::cout << "Результати збігаються: "
        << (functorResult == lambdaResult ? "так" : "ні")
        << '\n';

    return 0;
}