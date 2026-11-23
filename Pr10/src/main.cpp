#include "Database.h"
#include "SQLiteEventSourceRepository.h"
#include "LogEntryRepository.h"
#include "LogEntryState.h"
#include "PriorityStrategy.h"
#include "LogEventObserver.h"
#include "Timestamp.h"
#include <windows.h>

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    SetConsoleCP(65001);
	SetConsoleOutputCP(65001); 
    try {
        // Ініціалізація бази даних.
        initializeDatabase();

        SQLiteEventSourceRepository sourceRepository;
        LogEntryRepository logRepository;

        // Отримуємо наявне джерело подій.
        auto sources = sourceRepository.findAll();

        int sourceId = 0;

        auto activeSource = std::find_if(
            sources.begin(),
            sources.end(),
            [](const EventSource& source) {
                return source.isActive;
            }
        );

        // Якщо активного джерела немає, створюємо демонстраційне.
        if (activeSource != sources.end()) {
            sourceId = activeSource->id;
        }
        else {
            sourceId = sourceRepository.add({
                0,
                "Demo Source",
                "System",
                "Localhost",
                true
                });
        }

        // ================= OBSERVER =================

        LogEventPublisher publisher;

        auto audit = std::make_shared<AuditLogObserver>();
        auto statistics =
            std::make_shared<EventStatisticsObserver>();

        publisher.subscribe(audit);
        publisher.subscribe(statistics);

        std::cout << "\n=== OBSERVER ===\n";
        std::cout << "Підписників підключено.\n";

        // ================= CREATE LOG ENTRY =================

        LogEntry entry;

        entry.eventSourceId = sourceId;
        entry.timestamp = currentTimestamp();
        entry.level = "WARNING";
        entry.message = "Виявлено підозрілу активність.";
        entry.durationMs = 150;
        entry.importance = 8;
        entry.urgency = 6;
        entry.waitingTimeMinutes = 30;
        entry.status = "Новий";

        const int entryId = logRepository.add(entry);

        std::cout << "\n=== СТВОРЕННЯ ЗАПИСУ ===\n";
        std::cout << "Створено запис ID: " << entryId << '\n';

        publisher.publish({
            LogEventType::RecordReceived,
            entryId,
            entry.level,
            entry.message,
            "Новий запис додано."
            });

        // ================= STRATEGY + FACTORY METHOD =================

        std::cout << "\n=== STRATEGY ===\n";

        PriorityInput input{
            8.0,
            6.0,
            30.0
        };

        std::vector<std::unique_ptr<PriorityStrategyFactory>> factories;

        factories.push_back(
            std::make_unique<MaximumPriorityFactory>()
        );

        factories.push_back(
            std::make_unique<WeightedSumFactory>()
        );

        factories.push_back(
            std::make_unique<NormalizedWeightedSumFactory>()
        );

        for (const auto& factory : factories) {
            PriorityContext context(
                factory->createStrategy()
            );

            std::cout << context.strategyName()
                << ": "
                << context.calculate(input)
                << '\n';
        }

        // ================= STATE =================

        std::cout << "\n=== STATE ===\n";

        LogEntryWorkflow workflow(
            entryId,
            logRepository,
            publisher
        );

        std::cout << "Початковий стан: "
            << workflow.currentStatus() << '\n';

        workflow.check();

        std::cout << "Після перевірки: "
            << workflow.currentStatus() << '\n';

        workflow.process();

        std::cout << "Після опрацювання: "
            << workflow.currentStatus() << '\n';

        // Перевірка забороненого переходу.
        try {
            workflow.reject();
        }
        catch (const std::logic_error& ex) {
            std::cout << "Недопустимий перехід: "
                << ex.what() << '\n';
        }

        // ================= HISTORY =================

        std::cout << "\n=== ІСТОРІЯ СТАНІВ ===\n";

        const auto history =
            logRepository.findStatusHistory(entryId);

        for (const auto& item : history) {
            std::cout << item.previousStatus
                << " -> "
                << item.newStatus
                << " | "
                << item.changedAt
                << '\n';
        }

        // ================= OBSERVER UNSUBSCRIBE =================

        std::cout << "\n=== ВІДКЛЮЧЕННЯ OBSERVER ===\n";

        publisher.unsubscribe(audit);

        std::cout << "Журнал аудиту відключено.\n";

        // Створюємо ще один запис і відхиляємо його.
        LogEntry rejectedEntry = entry;
        rejectedEntry.message = "Запис для демонстрації відхилення.";
        rejectedEntry.status = "Новий";

        const int rejectedId = logRepository.add(rejectedEntry);

        publisher.publish({
            LogEventType::RecordReceived,
            rejectedId,
            rejectedEntry.level,
            rejectedEntry.message,
            "Новий запис додано."
            });

        LogEntryWorkflow rejectedWorkflow(
            rejectedId,
            logRepository,
            publisher
        );

        rejectedWorkflow.reject();

        std::cout << "Другий запис: "
            << rejectedWorkflow.currentStatus() << '\n';

        // Повертаємо підписника.
        publisher.subscribe(audit);

        std::cout << "Журнал аудиту знову підключено.\n";

        // ================= STATISTICS =================

        std::cout << "\n=== СТАТИСТИКА ===\n";

        statistics->printStatistics(std::cout);

        std::cout << "\nДемонстрацію завершено.\n";

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Помилка: " << ex.what() << '\n';
        return 1;
    }
}