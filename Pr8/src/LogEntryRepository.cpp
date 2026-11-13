#include "LogEntryRepository.h"

#include "Database.h"

#include <stdexcept>

int LogEntryRepository::add(const LogEntry& entry) {
    return static_cast<int>(getStorage().insert(entry));
}

std::vector<LogEntry> LogEntryRepository::findAll() const {
    return getStorage().get_all<LogEntry>();
}

std::vector<LogEntry> LogEntryRepository::findByLevel(const std::string& level) const {
    using namespace sqlite_orm;

    return getStorage().get_all<LogEntry>(
        where(c(&LogEntry::level) == level)
    );
}

std::vector<LogEntry> LogEntryRepository::findLatest(int maxCount) const {
    using namespace sqlite_orm;

    if (maxCount <= 0) {
        throw std::invalid_argument("The number of records must be positive.");
    }

    return getStorage().get_all<LogEntry>(
        order_by(&LogEntry::timestamp).desc(),
        limit(maxCount)
    );
}
