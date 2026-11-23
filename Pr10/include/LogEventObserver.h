#pragma once

#include <cstddef>
#include <fstream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

enum class LogEventType {
    RecordReceived,
    StatusChanged,
    ProcessingCompleted
};

struct LogEvent {
    LogEventType type;

    int logEntryId;

    std::string level;
    std::string message;
    std::string details;
};

class ILogEventObserver {
public:
    virtual ~ILogEventObserver() = default;

    virtual void update(const LogEvent& event) = 0;
};

class LogEventPublisher {
private:
    std::vector<std::weak_ptr<ILogEventObserver>> observers;

public:
    void subscribe(
        const std::shared_ptr<ILogEventObserver>& observer
    );

    void unsubscribe(
        const std::shared_ptr<ILogEventObserver>& observer
    );

    void publish(const LogEvent& event);
};

class AuditLogObserver final : public ILogEventObserver {
private:
    std::ofstream output;

public:
    explicit AuditLogObserver(
        const std::string& fileName = "audit.log"
    );

    void update(const LogEvent& event) override;
};

class EventStatisticsObserver final : public ILogEventObserver {
private:
    std::size_t receivedCount = 0;
    std::size_t statusChangedCount = 0;
    std::size_t completedCount = 0;

public:
    void update(const LogEvent& event) override;

    void printStatistics(std::ostream& stream) const;
};