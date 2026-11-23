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
            make_column("id", &EventSource::id,
                primary_key().autoincrement()),
            make_column("name", &EventSource::name),
            make_column("source_type", &EventSource::sourceType),
            make_column("location", &EventSource::location),
            make_column("is_active", &EventSource::isActive)
        ),

        make_table(
            "log_entries",
            make_column("id", &LogEntry::id,
                primary_key().autoincrement()),
            make_column("event_source_id", &LogEntry::eventSourceId),
            make_column("timestamp", &LogEntry::timestamp),
            make_column("level", &LogEntry::level),
            make_column("message", &LogEntry::message),
            make_column("duration_ms", &LogEntry::durationMs),

            make_column("importance", &LogEntry::importance,
                default_value(5)),
            make_column("urgency", &LogEntry::urgency,
                default_value(5)),
            make_column("waiting_time_minutes",
                &LogEntry::waitingTimeMinutes,
                default_value(0.0)),
            make_column("status", &LogEntry::status,
                default_value("Новий")),

            foreign_key(&LogEntry::eventSourceId)
            .references(&EventSource::id)
        ),

        make_table(
            "log_entry_status_history",
            make_column("id", &LogEntryStatusHistory::id,
                primary_key().autoincrement()),
            make_column("log_entry_id",
                &LogEntryStatusHistory::logEntryId),
            make_column("previous_status",
                &LogEntryStatusHistory::previousStatus),
            make_column("new_status",
                &LogEntryStatusHistory::newStatus),
            make_column("changed_at",
                &LogEntryStatusHistory::changedAt),

            foreign_key(&LogEntryStatusHistory::logEntryId)
            .references(&LogEntry::id)
        )
    );

    return storage;
}

void initializeDatabase();