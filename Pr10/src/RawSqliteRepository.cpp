#include "RawSqliteRepository.h"

#include <memory>
#include <stdexcept>
#include <string>

namespace {

using StatementPtr = std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)>;

std::string readTextColumn(sqlite3_stmt* statement, int columnIndex) {
    const auto* value = reinterpret_cast<const char*>(sqlite3_column_text(statement, columnIndex));
    return value != nullptr ? std::string(value) : std::string{};
}

} 

RawSqliteRepository::RawSqliteRepository(const std::string& databasePath) {
    const int result = sqlite3_open(databasePath.c_str(), &db_);

    if (result != SQLITE_OK) {
        const std::string message = db_ != nullptr ? sqlite3_errmsg(db_) : "Unknown SQLite error";
        if (db_ != nullptr) {
            sqlite3_close(db_);
            db_ = nullptr;
        }
        throw std::runtime_error("SQLite open error: " + message);
    }
}

RawSqliteRepository::~RawSqliteRepository() {
    if (db_ != nullptr) {
        sqlite3_close(db_);
    }
}

void RawSqliteRepository::checkResult(int result, const std::string& operation) const {
    if (result != SQLITE_OK) {
        throw std::runtime_error(operation + ": " + sqlite3_errmsg(db_));
    }
}

int RawSqliteRepository::add(const EventSource& source) {
    const char* sql =
        "INSERT INTO event_sources (name, source_type, location, is_active) "
        "VALUES (?, ?, ?, ?);";

    sqlite3_stmt* rawStatement = nullptr;
    checkResult(sqlite3_prepare_v2(db_, sql, -1, &rawStatement, nullptr), "Prepare INSERT");
    StatementPtr statement(rawStatement, sqlite3_finalize);

    checkResult(sqlite3_bind_text(statement.get(), 1, source.name.c_str(), -1, SQLITE_TRANSIENT), "Bind name");
    checkResult(sqlite3_bind_text(statement.get(), 2, source.sourceType.c_str(), -1, SQLITE_TRANSIENT), "Bind source type");
    checkResult(sqlite3_bind_text(statement.get(), 3, source.location.c_str(), -1, SQLITE_TRANSIENT), "Bind location");
    checkResult(sqlite3_bind_int(statement.get(), 4, source.isActive ? 1 : 0), "Bind active flag");

    const int stepResult = sqlite3_step(statement.get());
    if (stepResult != SQLITE_DONE) {
        throw std::runtime_error(std::string("Execute INSERT: ") + sqlite3_errmsg(db_));
    }

    return static_cast<int>(sqlite3_last_insert_rowid(db_));
}

std::optional<EventSource> RawSqliteRepository::findById(int id) const {
    const char* sql =
        "SELECT id, name, source_type, location, is_active "
        "FROM event_sources WHERE id = ?;";

    sqlite3_stmt* rawStatement = nullptr;
    checkResult(sqlite3_prepare_v2(db_, sql, -1, &rawStatement, nullptr), "Prepare SELECT");
    StatementPtr statement(rawStatement, sqlite3_finalize);

    checkResult(sqlite3_bind_int(statement.get(), 1, id), "Bind id");

    const int stepResult = sqlite3_step(statement.get());
    if (stepResult == SQLITE_DONE) {
        return std::nullopt;
    }
    if (stepResult != SQLITE_ROW) {
        throw std::runtime_error(std::string("Execute SELECT: ") + sqlite3_errmsg(db_));
    }

    EventSource source;
    source.id = sqlite3_column_int(statement.get(), 0);
    source.name = readTextColumn(statement.get(), 1);
    source.sourceType = readTextColumn(statement.get(), 2);
    source.location = readTextColumn(statement.get(), 3);
    source.isActive = sqlite3_column_int(statement.get(), 4) != 0;

    return source;
}
