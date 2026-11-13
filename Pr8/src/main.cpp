#include "Database.h"
#include "LogEntryRepository.h"
#include "RawSqliteRepository.h"
#include "SQLiteEventSourceRepository.h"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif
// Anonymous namespace to encapsulate helper functions and avoid name collisions.
namespace { 
// Helper function to print details of an EventSource.
void printSource(const EventSource& source) {
    std::cout << "ID=" << source.id
              << " | Назва=" << source.name
              << " | Тип=" << source.sourceType
              << " | Розташування=" << source.location
              << " | Активне=" << (source.isActive ? "так" : "ні") << '\n';
}
// Helper function to print details of a LogEntry.
void printLogEntry(const LogEntry& entry) {
    std::cout << "ID=" << entry.id
              << " | SourceID=" << entry.eventSourceId
              << " | " << entry.timestamp
              << " | " << entry.level
              << " | " << entry.message
              << " | " << entry.durationMs << " ms\n";
}
// Аdd demo data to the database using the provided repositories.
void seedDemoData(IEventSourceRepository& sourceRepository,
                  LogEntryRepository& logRepository) {
    auto& storage = getStorage();

    storage.remove_all<LogEntry>();
    storage.remove_all<EventSource>();

    const std::vector<EventSource> sources = {
        {0, "Authentication Service", "Backend", "10.0.0.10", true},
        {0, "Payment Gateway", "External API", "10.0.0.20", true},
        {0, "Temperature Sensor Hub", "IoT", "Laboratory A", true},
        {0, "Web Server", "Server", "192.168.1.10", true},
        {0, "Backup Agent", "System", "192.168.1.50", false}
    };

    std::vector<int> sourceIds;
    sourceIds.reserve(sources.size());

    for (const auto& source : sources) {
        sourceIds.push_back(sourceRepository.add(source));
    }

    const std::vector<LogEntry> entries = {
        {0, sourceIds[0], "2026-10-01 08:00:00", "INFO",    "Authentication service started", 18},
        {0, sourceIds[0], "2026-10-01 08:05:00", "WARNING", "Several failed login attempts", 40},
        {0, sourceIds[0], "2026-10-01 08:10:00", "ERROR",   "Token validation failed", 125},

        {0, sourceIds[1], "2026-10-01 09:00:00", "INFO",    "Payment request received", 56},
        {0, sourceIds[1], "2026-10-01 09:03:00", "ERROR",   "Payment provider timeout", 250},
        {0, sourceIds[1], "2026-10-01 09:05:00", "INFO",    "Payment status synchronized", 70},

        {0, sourceIds[2], "2026-10-01 10:00:00", "WARNING", "Temperature is above threshold", 35},
        {0, sourceIds[2], "2026-10-01 10:02:00", "ERROR",   "Sensor connection lost", 310},
        {0, sourceIds[2], "2026-10-01 10:04:00", "INFO",    "Sensor connection restored", 22},

        {0, sourceIds[3], "2026-10-01 11:00:00", "INFO",    "HTTP server started", 430},
        {0, sourceIds[3], "2026-10-01 11:05:00", "ERROR",   "Internal server error", 24},
        {0, sourceIds[3], "2026-10-01 11:08:00", "INFO",    "Configuration reloaded", 64},

        {0, sourceIds[4], "2026-10-01 12:00:00", "WARNING", "Backup started later than planned", 55},
        {0, sourceIds[4], "2026-10-01 12:15:00", "ERROR",   "Backup archive could not be created", 510},
        {0, sourceIds[0], "2026-10-01 12:20:00", "INFO",    "User session closed", 15}
    };

    for (const auto& entry : entries) {
        logRepository.add(entry);
    }
}

void compareRawApiAndOrm(IEventSourceRepository& sourceRepository) {
    std::cout << "\n========== ПОРІВНЯННЯ SQLITE C API ТА ORM ==========\n";

	{// Demonstrate raw SQLite C API operations using the RawSqliteRepository.
        RawSqliteRepository rawRepository(DATABASE_FILE);
        const EventSource rawSource{
            0, "Source created through SQLite C API", "Comparison", "localhost:7001", true
        };

        const int rawId = rawRepository.add(rawSource);
        const auto rawResult = rawRepository.findById(rawId);

        std::cout << "SQLite C API: додано та прочитано запис.\n";
        if (rawResult) {
            printSource(*rawResult);
        }
    }
	// Demonstrate ORM operations using the SQLiteEventSourceRepository.
    const EventSource ormSource{
        0, "Source created through sqlite_orm", "Comparison", "localhost:7002", true
    };
    
    const int ormId = sourceRepository.add(ormSource);
    const auto ormResult = sourceRepository.findById(ormId);

    std::cout << "sqlite_orm: додано та прочитано запис.\n";
    if (ormResult) {
        printSource(*ormResult);
    }
}
// Demonstrate CRUD operations using the provided repository.
void demonstrateRepositoryCrud(IEventSourceRepository& repository) {
    std::cout << "\n========== REPOSITORY ТА CRUD ==========\n";

    const int id = repository.add(
        EventSource{0, "Temporary source", "Test", "Initial location", true}
    );
    std::cout << "CREATE: додано джерело з ID=" << id << '\n';

    const auto found = repository.findById(id);
    if (found) {
        std::cout << "READ: знайдено запис: ";
        printSource(*found);
    }

    const auto allSources = repository.findAll();
    std::cout << "READ ALL: кількість джерел подій = " << allSources.size() << '\n';

    if (auto sourceToUpdate = repository.findById(id)) {
        sourceToUpdate->location = "Updated location";
        repository.update(*sourceToUpdate);
    }

    if (const auto updated = repository.findById(id)) {
        std::cout << "UPDATE: нове розташування = " << updated->location << '\n';
    }

    repository.remove(id);
    std::cout << "DELETE: тимчасове джерело видалено.\n";
}
// Demonstrate various queries using the LogEntryRepository.
void demonstrateQueries(const LogEntryRepository& logRepository) {
    using namespace sqlite_orm;

    auto& storage = getStorage();

    std::cout << "\n========== ФІЛЬТРАЦІЯ: ERROR ==========\n";
    const auto errors = logRepository.findByLevel("ERROR");
    for (const auto& entry : errors) {
        printLogEntry(entry);
    }
    std::cout << "Кількість записів рівня ERROR: " << errors.size() << '\n';

    std::cout << "\n========== СОРТУВАННЯ ТА LIMIT: ОСТАННІ 5 ЗАПИСІВ ==========\n";
    const auto latest = logRepository.findLatest(5);
    for (const auto& entry : latest) {
        printLogEntry(entry);
    }

    std::cout << "\n========== АГРЕГАТНІ ОБЧИСЛЕННЯ ==========\n";
    const auto totalCount = storage.count<LogEntry>();
    const auto totalDuration = storage.sum(&LogEntry::durationMs);
    const double averageDuration = storage.avg(&LogEntry::durationMs);

    std::cout << "COUNT: кількість записів = " << totalCount << '\n';
    std::cout << "SUM: загальна тривалість = "
              << (totalDuration ? *totalDuration : 0) << " ms\n";
    std::cout << "AVG: середня тривалість = "
              << std::fixed << std::setprecision(2) << averageDuration << " ms\n";

    std::cout << "\n========== GROUP BY: КІЛЬКІСТЬ ЗАПИСІВ ЗА РІВНЕМ ==========\n";
    const auto groupedCounts = storage.select(
        columns(&LogEntry::level, count(&LogEntry::id)),
        group_by(&LogEntry::level)
    );

    for (const auto& group : groupedCounts) {
        std::cout << "Рівень " << std::get<0>(group)
                  << ": " << std::get<1>(group) << " запис(ів)\n";
    }

    std::cout << "\n========== INNER JOIN: ДЖЕРЕЛО + ЗАПИС ЖУРНАЛУ ==========\n";
    const auto joinedRows = storage.select(
        columns(
            &EventSource::name,
            &LogEntry::timestamp,
            &LogEntry::level,
            &LogEntry::message,
            &LogEntry::durationMs
        ),
        inner_join<LogEntry>(
            on(c(&EventSource::id) == &LogEntry::eventSourceId)
        )
    );

    for (const auto& row : joinedRows) {
        std::cout << "Джерело=" << std::get<0>(row)
                  << " | " << std::get<1>(row)
                  << " | " << std::get<2>(row)
                  << " | " << std::get<3>(row)
                  << " | " << std::get<4>(row) << " ms\n";
    }
}
// Demonstrate transaction handling, including commit and rollback scenarios.
void demonstrateTransactions() {
    auto& storage = getStorage();

    std::cout << "\n========== ТРАНЗАКЦІЯ: УСПІШНИЙ COMMIT ==========\n";
    const int sourcesBeforeCommit = storage.count<EventSource>();
    const int logsBeforeCommit = storage.count<LogEntry>();

    int committedSourceId = 0;
    int committedLogId = 0;

    const bool committed = storage.transaction([&]() {
        committedSourceId = static_cast<int>(storage.insert(
            EventSource{0, "Transaction success source", "Transaction", "10.0.0.80", true}
        ));

        committedLogId = static_cast<int>(storage.insert(
            LogEntry{
                0,
                committedSourceId,
                "2026-10-01 13:00:00",
                "INFO",
                "Initial event was registered in the same transaction",
                80
            }
        ));

        return true; 
    });

    std::cout << "До транзакції: джерел=" << sourcesBeforeCommit
              << ", записів журналу=" << logsBeforeCommit << '\n';
    std::cout << "Після транзакції: джерел=" << storage.count<EventSource>()
              << ", записів журналу=" << storage.count<LogEntry>() << '\n';
    std::cout << "COMMIT виконано: " << (committed ? "так" : "ні")
              << "; нові ID: source=" << committedSourceId
              << ", log=" << committedLogId << '\n';

    std::cout << "\n========== ТРАНЗАКЦІЯ: ПОМИЛКА ТА ROLLBACK ==========\n";
    const int sourcesBeforeRollback = storage.count<EventSource>();
    const int logsBeforeRollback = storage.count<LogEntry>();
    int rolledBackSourceId = 0;

	// Attempt a transaction that will fail due to a foreign key constraint violation, triggering a rollback.
    try {
        storage.transaction([&]() {
            rolledBackSourceId = static_cast<int>(storage.insert(
                EventSource{0, "Transaction rollback source", "Transaction", "10.0.0.90", true}
            ));

            storage.insert(
                LogEntry{
                    0,
                    999999,
                    "2026-10-01 13:10:00",
                    "ERROR",
                    "This record references a non-existing source",
                    999
                }
            );

            return true;
        });
    } catch (const std::exception& ex) {
        std::cout << "Очікувана помилка SQLite: " << ex.what() << '\n';
    }
	// Check if the source added in the failed transaction still exists, which it should not due to rollback.
    const bool rolledBackSourceExists = storage.get_optional<EventSource>(rolledBackSourceId).has_value();

    std::cout << "До невдалої транзакції: джерел=" << sourcesBeforeRollback
              << ", записів журналу=" << logsBeforeRollback << '\n';
    std::cout << "Після невдалої транзакції: джерел=" << storage.count<EventSource>()
              << ", записів журналу=" << storage.count<LogEntry>() << '\n';
    std::cout << "Джерело, додане першою операцією, існує після ROLLBACK: "
              << (rolledBackSourceExists ? "ТАК (помилка)" : "НІ (відкат підтверджено)") << '\n';

    std::cout << "\n========== ДВІ ОПЕРАЦІЇ БЕЗ ЯВНОЇ ТРАНЗАКЦІЇ ==========\n";
    const int sourcesBeforeNoTransaction = storage.count<EventSource>();
    const int logsBeforeNoTransaction = storage.count<LogEntry>();

    const int partiallySavedSourceId = static_cast<int>(storage.insert(
        EventSource{0, "Source without explicit transaction", "Comparison", "10.0.0.100", true}
    ));
	// Attempt to insert a LogEntry that references a non-existing EventSource, which should fail.
    try {
        storage.insert(
            LogEntry{
                0,
                999999,
                "2026-10-01 13:20:00",
                "ERROR",
                "This insert must fail because the source does not exist",
                1000
            }
        );
    } catch (const std::exception& ex) {
        std::cout << "Друга операція завершилася помилкою: " << ex.what() << '\n';
    }

    const bool partiallySavedSourceExists =
        storage.get_optional<EventSource>(partiallySavedSourceId).has_value();

    std::cout << "До операцій: джерел=" << sourcesBeforeNoTransaction
              << ", записів журналу=" << logsBeforeNoTransaction << '\n';
    std::cout << "Після помилки без транзакції: джерел=" << storage.count<EventSource>()
              << ", записів журналу=" << storage.count<LogEntry>() << '\n';
    std::cout << "Перша операція залишилася в БД: "
              << (partiallySavedSourceExists ? "так" : "ні") << '\n';

    if (partiallySavedSourceExists) {
        storage.remove<EventSource>(partiallySavedSourceId);
    }
}

} 

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    try {
        std::cout << "ЛАБОРАТОРНА РОБОТА №8. ВАРІАНТ 1\n";
        std::cout << "Система обробки журналів подій\n";

        initializeDatabase();

        SQLiteEventSourceRepository sqliteRepository;
        IEventSourceRepository& sourceRepository = sqliteRepository;
        LogEntryRepository logRepository;

        seedDemoData(sourceRepository, logRepository);
        std::cout << "\nТестові дані підготовлено: "
                  << getStorage().count<EventSource>() << " джерел та "
                  << getStorage().count<LogEntry>() << " записів журналу.\n";

        compareRawApiAndOrm(sourceRepository);
        demonstrateRepositoryCrud(sourceRepository);
        demonstrateQueries(logRepository);
        demonstrateTransactions();

        std::cout << "\n========== ЗАВЕРШЕННЯ ==========\n";
        std::cout << "Усі демонстраційні сценарії виконано.\n";
        std::cout << "Файл бази даних: " << DATABASE_FILE << '\n';
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Критична помилка програми: " << ex.what() << '\n';
        return 1;
    }
}
