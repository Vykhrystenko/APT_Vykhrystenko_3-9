#pragma once

#include "EventModels.h"

#include <string>
#include <vector>

// Окремий компонент доступу до залежної сутності LogEntry.
class LogEntryRepository {
public:
    int add(const LogEntry& entry);
    std::vector<LogEntry> findAll() const;
    std::vector<LogEntry> findByLevel(const std::string& level) const;
    std::vector<LogEntry> findLatest(int maxCount) const;
};
