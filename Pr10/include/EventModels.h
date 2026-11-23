#pragma once

#include <string>

struct EventSource {
    int id = 0;
    std::string name;
    std::string sourceType;
    std::string location;
    bool isActive = true;
};

struct LogEntry {
    int id = 0;
    int eventSourceId = 0;
    std::string timestamp;
    std::string level;
    std::string message;
    int durationMs = 0;

    int importance = 5;
    int urgency = 5;
    double waitingTimeMinutes = 0.0;

    std::string status = "Новий";
};

struct LogEntryStatusHistory {
    int id = 0;
    int logEntryId = 0;

    std::string previousStatus;
    std::string newStatus;
    std::string changedAt;
};