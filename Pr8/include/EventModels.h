#pragma once

#include <string>

// Основна сутність: джерело подій.
struct EventSource {
    int id = 0;
    std::string name;
    std::string sourceType;
    std::string location;
    bool isActive = true;
};

// Залежна сутність: окремий запис журналу.
struct LogEntry {
    int id = 0;
    int eventSourceId = 0;
    std::string timestamp;
    std::string level;
    std::string message;
    int durationMs = 0;
};
