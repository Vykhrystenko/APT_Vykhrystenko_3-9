#pragma once

#include "LogEntryRepository.h"
#include "LogEventObserver.h"

#include <memory>
#include <string>

class LogEntryWorkflow;

class LogEntryState {
public:
    virtual ~LogEntryState() = default;

    virtual std::string name() const = 0;

    virtual void check(LogEntryWorkflow& workflow) const = 0;

    virtual void process(LogEntryWorkflow& workflow) const = 0;

    virtual void reject(LogEntryWorkflow& workflow) const = 0;
};

class NewLogEntryState final : public LogEntryState {
public:
    std::string name() const override;

    void check(LogEntryWorkflow& workflow) const override;

    void process(LogEntryWorkflow& workflow) const override;

    void reject(LogEntryWorkflow& workflow) const override;
};

class CheckedLogEntryState final : public LogEntryState {
public:
    std::string name() const override;

    void check(LogEntryWorkflow& workflow) const override;

    void process(LogEntryWorkflow& workflow) const override;

    void reject(LogEntryWorkflow& workflow) const override;
};

class ProcessedLogEntryState final : public LogEntryState {
public:
    std::string name() const override;

    void check(LogEntryWorkflow& workflow) const override;

    void process(LogEntryWorkflow& workflow) const override;

    void reject(LogEntryWorkflow& workflow) const override;
};

class RejectedLogEntryState final : public LogEntryState {
public:
    std::string name() const override;

    void check(LogEntryWorkflow& workflow) const override;

    void process(LogEntryWorkflow& workflow) const override;

    void reject(LogEntryWorkflow& workflow) const override;
};

class LogEntryWorkflow {
private:
    int entryId;

    LogEntryRepository& repository;
    LogEventPublisher& publisher;

    std::unique_ptr<LogEntryState> state;

public:
    LogEntryWorkflow(
        int id,
        LogEntryRepository& repository,
        LogEventPublisher& publisher
    );

    void check();

    void process();

    void reject();

    std::string currentStatus() const;

    void transitionTo(std::unique_ptr<LogEntryState> newState);
};