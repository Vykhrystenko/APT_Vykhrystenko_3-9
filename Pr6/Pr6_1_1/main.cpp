#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <vector>

const int ITERATIONS = 100000;
const int RUNS = 10; 

int counter_mutex = 0;
std::mutex counterMutex;

void increment_mutex() {
    for (int i = 0; i < ITERATIONS; ++i) {
        std::lock_guard<std::mutex> lock(counterMutex);
        ++counter_mutex;
    }
}

std::atomic<int> counter_atomic{ 0 };

void increment_atomic() {
    for (int i = 0; i < ITERATIONS; ++i) {
        ++counter_atomic;
    }
}

int main() {
	std::locale::global(std::locale("uk_UA.UTF-8"));
   
    std::cout << std::fixed;
    std::cout.precision(3);

    std::vector<double> mutex_times(RUNS);
    std::vector<double> atomic_times(RUNS);

    std::cout << "Виконання замірів для std::mutex (" << RUNS << " прогонів)...\n";
    for (int r = 0; r < RUNS; ++r) {
        counter_mutex = 0;

        auto start = std::chrono::steady_clock::now();

        std::thread t1(increment_mutex);
        std::thread t2(increment_mutex);
        t1.join();
        t2.join();

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        mutex_times[r] = elapsed.count();
    }

    std::cout << "Виконання замірів для std::atomic (" << RUNS << " прогонів)...\n";
    for (int r = 0; r < RUNS; ++r) {
        counter_atomic.store(0);

        auto start = std::chrono::steady_clock::now();

        std::thread t1(increment_atomic);
        std::thread t2(increment_atomic);
        t1.join();
        t2.join();

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        atomic_times[r] = elapsed.count();
    }

	// Виведення результатів для std::mutex
    double mutex_sum = 0;
    std::cout << "РЕЗУЛЬТАТИ: std::mutex\n";
    for (int i = 0; i < RUNS; ++i) {
        std::cout << "Прогін " << (i + 1) << ": " << mutex_times[i] << " мс\n";
        mutex_sum += mutex_times[i];
    }
    double mutex_avg = mutex_sum / RUNS;
    std::cout << "Середній час (mutex): " << mutex_avg << " мс\n";

    // Виведення результатів для std::atomic
    double atomic_sum = 0;
    std::cout << "\nРЕЗУЛЬТАТИ: std::atomic\n";
    for (int i = 0; i < RUNS; ++i) {
        std::cout << "Прогін " << (i + 1) << ": " << atomic_times[i] << " мс\n";
        atomic_sum += atomic_times[i];
    }
    double atomic_avg = atomic_sum / RUNS;
    std::cout << "Середній час (atomic): " << atomic_avg << " мс\n";

    return 0;
}