#include "LogEventObserver.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace {

    // Перетворення типу події на текст.
    const char* eventTypeToString(LogEventType type) {
        switch (type) {
        case LogEventType::RecordReceived:
            return "RecordReceived";

        case LogEventType::StatusChanged:
            return "StatusChanged";

        case LogEventType::ProcessingCompleted:
            return "ProcessingCompleted";
        }

        return "Unknown";
    }

}

// ================= LOG EVENT PUBLISHER =================

// Додавання спостерігача.
void LogEventPublisher::subscribe(
    const std::shared_ptr<ILogEventObserver>& observer
) {
    if (!observer) {
        throw std::invalid_argument(
            "Observer cannot be null."
        );
    }

    // Видаляємо неактивні посилання.
    observers.erase(
        std::remove_if(
            observers.begin(),
            observers.end(),
            [](const std::weak_ptr<ILogEventObserver>& item) {
                return item.expired();
            }
        ),
        observers.end()
    );

    // Перевіряємо, чи вже підписаний спостерігач.
    for (const auto& item : observers) {
        if (item.lock() == observer) {
            return;
        }
    }

    observers.push_back(observer);
}

// Відписка спостерігача.
void LogEventPublisher::unsubscribe(
    const std::shared_ptr<ILogEventObserver>& observer
) {
    observers.erase(
        std::remove_if(
            observers.begin(),
            observers.end(),
            [&observer](const std::weak_ptr<ILogEventObserver>& item) {
                const auto current = item.lock();

                return !current || current == observer;
            }
        ),
        observers.end()
    );
}

// Розсилання події всім активним спостерігачам.
void LogEventPublisher::publish(const LogEvent& event) {
    std::vector<std::shared_ptr<ILogEventObserver>> activeObservers;

    // Отримуємо активних спостерігачів.
    for (auto it = observers.begin(); it != observers.end();) {
        if (auto observer = it->lock()) {
            activeObservers.push_back(observer);
            ++it;
        }
        else {
            it = observers.erase(it);
        }
    }

    // Повідомляємо спостерігачів про подію.
    for (const auto& observer : activeObservers) {
        observer->update(event);
    }
}

// ================= AUDIT LOG OBSERVER =================

AuditLogObserver::AuditLogObserver(
    const std::string& fileName
)
    : output(fileName, std::ios::out | std::ios::app) {

    if (!output.is_open()) {
        throw std::runtime_error(
            "Cannot open audit log file."
        );
    }
}

// Запис події до файлу аудиту.
void AuditLogObserver::update(const LogEvent& event) {
    output
        << "Event: " << eventTypeToString(event.type) << '\n'
        << "Log entry ID: " << event.logEntryId << '\n'
        << "Level: " << event.level << '\n'
        << "Message: " << event.message << '\n'
        << "Details: " << event.details << '\n'
        << "-----------------------------\n";

    output.flush();
}

// ================= EVENT STATISTICS OBSERVER =================

// Оновлення статистики відповідно до типу події.
void EventStatisticsObserver::update(const LogEvent& event) {
    switch (event.type) {
    case LogEventType::RecordReceived:
        ++receivedCount;
        break;

    case LogEventType::StatusChanged:
        ++statusChangedCount;
        break;

    case LogEventType::ProcessingCompleted:
        ++completedCount;
        break;
    }
}

// Виведення статистики.
void EventStatisticsObserver::printStatistics(
    std::ostream& stream
) const {
    stream << "Received events: "
        << receivedCount << '\n';

    stream << "Status changes: "
        << statusChangedCount << '\n';

    stream << "Completed processing: "
        << completedCount << '\n';
}