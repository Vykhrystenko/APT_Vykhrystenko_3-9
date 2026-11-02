#include <iostream>
#include <vector>
#include <numeric>
#include <future>
#include <chrono>
#include <random>
// Функція для обчислення суми елементів вектора послідовно
long long sumSequential(const std::vector<int>& data) {
    long long totalSum = 0;
    for (int val : data) {
        totalSum += val;
    }
    return totalSum;
}
// Функція для обчислення суми елементів вектора в межах блоку
long long sumBlock(const std::vector<int>& data, size_t start, size_t end) {
    long long blockSum = 0;
    for (size_t i = start; i < end; ++i) {
        blockSum += data[i];
    }
    return blockSum;
}
// Функція для обчислення суми елементів вектора паралельно з використанням std::async
long long sumParallel(const std::vector<int>& data, size_t numTasks) {

    if (data.empty() || numTasks == 0) return 0;

    if (data.size() < numTasks) {
        numTasks = data.size();
    }

	// Розрахунок розміру блоку для кожного завдання
    size_t chunkSize = data.size() / numTasks;

	// Вектор для збереження майбутніх результатів
    std::vector<std::future<long long>> futures;

	// Резервування місця у векторі для майбутніх результатів
    futures.reserve(numTasks);

	// Створення асинхронних завдань для обчислення суми блоків
    for (size_t i = 0; i < numTasks; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numTasks - 1) ? data.size() : (start + chunkSize);
        futures.push_back(
            std::async(std::launch::async, sumBlock, std::cref(data), start, end)
        );
    }
	// Очікування завершення всіх асинхронних завдань та об'єднання результатів
    long long totalSum = 0;
    for (auto& fut : futures) {
        totalSum += fut.get();
    }

    return totalSum;
}

int main() {
    std::vector<size_t> testSizes = { 10'000'000, 50'000'000, 100'000'000 };
    size_t numTasks = 4;

    std::cout << "Data Size\tSeq Time (ms)\tPar Time (ms)\tSpeedup\t\tMatch\n\n";

    for (size_t n : testSizes) {
		// Генерація випадкових даних для тестування
        std::vector<int> data(n);
        std::mt19937 rng(42);
        std::uniform_int_distribution<int> dist(1, 100);
        for (size_t i = 0; i < n; ++i) {
            data[i] = dist(rng);
        }

        auto startSeq = std::chrono::steady_clock::now();
        long long seqResult = sumSequential(data);
        auto endSeq = std::chrono::steady_clock::now();
        auto seqDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endSeq - startSeq).count();

        auto startPar = std::chrono::steady_clock::now();
        long long parResult = sumParallel(data, numTasks);
        auto endPar = std::chrono::steady_clock::now();
        auto parDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endPar - startPar).count();
		// Обчислення прискорення та перевірка відповідності результатів
        double speedup = (parDuration > 0) ? (static_cast<double>(seqDuration) / parDuration) : 0.0;
        bool isMatch = (seqResult == parResult);

        std::cout << n << "\t"
            << seqDuration << "\t\t"
            << parDuration << "\t\t"
            << speedup << "x\t\t"
            << (isMatch ? "PASS" : "FAIL") << "\n";
    }

    return 0;
}