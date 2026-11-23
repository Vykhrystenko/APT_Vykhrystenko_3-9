#include "LogEntryRepository.h"

#include "Database.h"
#include "Timestamp.h"

#include <stdexcept>

int LogEntryRepository::add(const LogEntry& entry) {
    return static_cast<int>(getStorage().insert(entry));
}

std::optional<LogEntry> LogEntryRepository::findById(int id) const {
    return getStorage().get_optional<LogEntry>(id);
}

std::vector<LogEntry> LogEntryRepository::findAll() const {
    return getStorage().get_all<LogEntry>();
}

std::vector<LogEntry> LogEntryRepository::findByLevel(
    const std::string& level
) const {
    using namespace sqlite_orm;

    return getStorage().get_all<LogEntry>(
        where(c(&LogEntry::level) == level)
    );
}

std::vector<LogEntry> LogEntryRepository::findLatest(
    int maxCount
) const {
    using namespace sqlite_orm;

    if (maxCount <= 0) {
        throw std::invalid_argument(
            "The number of records must be positive."
        );
    }

    return getStorage().get_all<LogEntry>(
        order_by(&LogEntry::timestamp).desc(),
        limit(maxCount)
    );
}

void LogEntryRepository::changeStatusWithHistory(
    int id,
    const std::string& expectedStatus,
    const std::string& newStatus
) {
    if (id <= 0 || newStatus.empty()) {
        throw std::invalid_argument("Invalid status update parameters.");
    }

    auto& storage = getStorage();

    storage.transaction([&]() {
        auto entry = storage.get_optional<LogEntry>(id);

        if (!entry) {
            throw std::runtime_error(
                "LogEntry with the specified ID does not exist."
            );
        }

        if (entry->status != expectedStatus) {
            throw std::runtime_error(
                "The current status does not match the expected status."
            );
        }

        if (entry->status == newStatus) {
            throw std::invalid_argument(
                "The new status is identical to the current status."
            );
        }

        entry->status = newStatus;
        storage.update(*entry);

        LogEntryStatusHistory history{
            0,
            id,
            expectedStatus,
            newStatus,
            currentTimestamp()
        };

        storage.insert(history);

        return true;
        });
}

std::vector<LogEntryStatusHistory>
LogEntryRepository::findStatusHistory(int logEntryId) const {
    using namespace sqlite_orm;

    return getStorage().get_all<LogEntryStatusHistory>(
        where(c(&LogEntryStatusHistory::logEntryId) == logEntryId),
        order_by(&LogEntryStatusHistory::id).asc()
    );
}
