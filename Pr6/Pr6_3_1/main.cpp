#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

// Структура для лог-повідомлення
struct LogMessage
{
    int id = 0;
    std::string timestamp = "";
    std::string text = "";
};

std::queue<LogMessage> logQueue;
std::mutex queueMutex;
std::mutex outputMutex;
std::mutex fileMutex;
std::condition_variable condition;
bool finished = false;
const int LOG_COUNT = 12;

// Потік-виробник
void producer()
{
    const std::string logTemplates[] =
    {
        "INFO: User successfully logged into the system",
        "WARNING: High CPU temperature detected",
        "ERROR: Failed to establish database connection",
        "DEBUG: Cache refreshed successfully"
    };
	// Генеруємо лог-повідомлення
    for (int i = 1; i <= LOG_COUNT; ++i)
    {
		// Отримуємо поточний час у форматі "YYYY-MM-DD HH:MM:SS"
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);//
		// Створюємо структуру tm для зберігання локального часу
        std::tm bt{};
#if defined(_MSC_VER)
		localtime_s(&bt, &in_time_t); // Безпечний аналог для Visual Studio (Windows)
#else
        localtime_r(&in_time_t, &bt); // Безпечний аналог для POSIX-систем (Linux/macOS)
#endif
		// Форматуємо час у рядок
        std::stringstream ss;
        ss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S");

        LogMessage log =
        {
            i,
            ss.str(),
            logTemplates[(i - 1) % 4]
        };
		// Додаємо лог-повідомлення у чергу
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            logQueue.push(log);
        }
		// Виводимо повідомлення про згенероване лог-повідомлення
        {
            std::lock_guard<std::mutex> lock(outputMutex);
            std::cout << "[Producer] Generated log #" << log.id
                << " [" << log.timestamp << "]\n";
        }
		// Повідомляємо споживачів про наявність нового лог-повідомлення та імітуємо затримку між генерацією логів
        condition.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
	// Встановлюємо прапорець завершення роботи виробника
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        finished = true;
    }
	// Повідомляємо споживачів, що виробник завершив роботу
    condition.notify_all();
	// Виводимо повідомлення про завершення роботи виробника
    {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "[Producer] Log generation finished\n";
    }
}

// Потік-споживач
void consumer(int consumerId)
{
    std::ofstream outFile("system_logs.txt", std::ios::app);
    
    while (true)
    {
        LogMessage log;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
			// Очікуємо, поки черга не стане непорожньою або виробник не завершить роботу
            condition.wait(lock, []()
                {
                    return !logQueue.empty() || finished;
                });
			// Якщо черга порожня і виробник завершив роботу, виходимо з циклу
            if (logQueue.empty() && finished)
            {
                break;
            }
			// Витягуємо лог-повідомлення з черги
            log = logQueue.front();
            logQueue.pop();
        }
		// Імітуємо обробку лог-повідомлення
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
		// Записуємо оброблене лог-повідомлення у файл
        {
            std::lock_guard<std::mutex> lock(fileMutex);
            if (outFile.is_open())
            {
                outFile << "[" << log.timestamp << "] [Consumer " << consumerId 
                    << "] ID: " << log.id << " - " << log.text << "\n";
            }
        }

        {
            std::lock_guard<std::mutex> lock(outputMutex);
            std::cout << "[Consumer " << consumerId << "] Processed and saved log #"
                << log.id << '\n';
        }
    }
	// Закриваємо файл після завершення обробки
    if (outFile.is_open())
    {
        outFile.close();
    }
	// Виводимо повідомлення про завершення роботи споживача
    {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "[Consumer " << consumerId << "] Finished\n";
    }
}

int main()
{
    auto start_time = std::chrono::steady_clock::now();

    std::thread producerThread(producer);
    std::thread consumer1(consumer, 1);
    std::thread consumer2(consumer, 2);

    producerThread.join();
    consumer1.join();
    consumer2.join();

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time = end_time - start_time;

    std::cout << "\nAll logs have been processed and saved to 'system_logs.txt'.\n";
    std::cout << "Elapsed time: " << elapsed_time.count() << " ms\n";

    return 0;
}