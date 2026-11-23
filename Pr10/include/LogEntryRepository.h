#pragma once

#include "EventModels.h"

#include <optional>
#include <string>
#include <vector>

class LogEntryRepository {
public:
    int add(const LogEntry& entry);

    std::optional<LogEntry> findById(int id) const;

    std::vector<LogEntry> findAll() const;

    std::vector<LogEntry> findByLevel(
        const std::string& level
    ) const;

    std::vector<LogEntry> findLatest(int maxCount) const;

    void changeStatusWithHistory(
        int id,
        const std::string& expectedStatus,
        const std::string& newStatus
    );

    std::vector<LogEntryStatusHistory> findStatusHistory(
        int logEntryId
    ) const;
};