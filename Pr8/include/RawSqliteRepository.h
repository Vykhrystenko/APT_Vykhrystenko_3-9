#pragma once

#include "EventModels.h"
#include <optional>
#include <string>
#include <sqlite3.h>

// A repository class that provides raw SQLite operations for EventSource entities.
class RawSqliteRepository {
public:
    explicit RawSqliteRepository(const std::string& databasePath);
    ~RawSqliteRepository();

    RawSqliteRepository(const RawSqliteRepository&) = delete;
    RawSqliteRepository& operator=(const RawSqliteRepository&) = delete;

    int add(const EventSource& source);
    std::optional<EventSource> findById(int id) const;

private:
    sqlite3* db_ = nullptr;

    void checkResult(int result, const std::string& operation) const;
};
