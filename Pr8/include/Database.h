#pragma once

#include "EventModels.h"
#include <sqlite_orm/sqlite_orm.h>

inline constexpr const char* DATABASE_FILE = "event_logs.db";

inline auto& getStorage() {
    using namespace sqlite_orm;

    static auto storage = make_storage(
        DATABASE_FILE,

        make_table(
            "event_sources",
            make_column("id", &EventSource::id, primary_key().autoincrement()),
            make_column("name", &EventSource::name),
            make_column("source_type", &EventSource::sourceType),
            make_column("location", &EventSource::location),
            make_column("is_active", &EventSource::isActive)
        ),

        make_table(
            "log_entries",
            make_column("id", &LogEntry::id, primary_key().autoincrement()),
            make_column("event_source_id", &LogEntry::eventSourceId),
            make_column("timestamp", &LogEntry::timestamp),
            make_column("level", &LogEntry::level),
            make_column("message", &LogEntry::message),
            make_column("duration_ms", &LogEntry::durationMs),
            foreign_key(&LogEntry::eventSourceId).references(&EventSource::id)
        )
    );

    return storage;
}

void initializeDatabase();
