#include "LogEntryState.h"
#include <stdexcept>


std::string NewLogEntryState::name() const {
    return "Новий";
}

void NewLogEntryState::check(LogEntryWorkflow& workflow) const {
    workflow.transitionTo(
        std::make_unique<CheckedLogEntryState>()
    );
}

void NewLogEntryState::process(LogEntryWorkflow&) const {
    throw std::logic_error(
        "A new entry must be checked before processing."
    );
}

void NewLogEntryState::reject(LogEntryWorkflow& workflow) const {
    workflow.transitionTo(
        std::make_unique<RejectedLogEntryState>()
    );
}


std::string CheckedLogEntryState::name() const {
    return "Перевірений";
}

void CheckedLogEntryState::check(LogEntryWorkflow&) const {
    throw std::logic_error(
        "The entry has already been checked."
    );
}

void CheckedLogEntryState::process(LogEntryWorkflow& workflow) const {
    workflow.transitionTo(
        std::make_unique<ProcessedLogEntryState>()
    );
}

void CheckedLogEntryState::reject(LogEntryWorkflow& workflow) const {
    workflow.transitionTo(
        std::make_unique<RejectedLogEntryState>()
    );
}


std::string ProcessedLogEntryState::name() const {
    return "Опрацьований";
}

void ProcessedLogEntryState::check(LogEntryWorkflow&) const {
    throw std::logic_error(
        "The entry has already been processed."
    );
}

void ProcessedLogEntryState::process(LogEntryWorkflow&) const {
    throw std::logic_error(
        "The entry has already been processed."
    );
}

void ProcessedLogEntryState::reject(LogEntryWorkflow&) const {
    throw std::logic_error(
        "A processed entry cannot be rejected."
    );
}


std::string RejectedLogEntryState::name() const {
    return "Відхилений";
}

void RejectedLogEntryState::check(LogEntryWorkflow&) const {
    throw std::logic_error(
        "A rejected entry cannot be checked."
    );
}

void RejectedLogEntryState::process(LogEntryWorkflow&) const {
    throw std::logic_error(
        "A rejected entry cannot be processed."
    );
}

void RejectedLogEntryState::reject(LogEntryWorkflow&) const {
    throw std::logic_error(
        "The entry has already been rejected."
    );
}

LogEntryWorkflow::LogEntryWorkflow(
    int id,
    LogEntryRepository& repository,
    LogEventPublisher& publisher
)
    : entryId(id),
    repository(repository),
    publisher(publisher) {

    const auto entry = repository.findById(id);

    if (!entry) {
        throw std::runtime_error(
            "LogEntry with the specified ID does not exist."
        );
    }

    if (entry->status == "Новий") {
        state = std::make_unique<NewLogEntryState>();
    }
    else if (entry->status == "Перевірений") {
        state = std::make_unique<CheckedLogEntryState>();
    }
    else if (entry->status == "Опрацьований") {
        state = std::make_unique<ProcessedLogEntryState>();
    }
    else if (entry->status == "Відхилений") {
        state = std::make_unique<RejectedLogEntryState>();
    }
    else {
        throw std::runtime_error(
            "Unknown LogEntry status in database."
        );
    }
}

void LogEntryWorkflow::check() {
    state->check(*this);
}

void LogEntryWorkflow::process() {
    state->process(*this);
}

void LogEntryWorkflow::reject() {
    state->reject(*this);
}

std::string LogEntryWorkflow::currentStatus() const {
    return state->name();
}

void LogEntryWorkflow::transitionTo(
    std::unique_ptr<LogEntryState> newState
) {
    if (!newState) {
        throw std::invalid_argument("New state cannot be null.");
    }

    const std::string previousStatus = state->name();
    const std::string nextStatus = newState->name();

    repository.changeStatusWithHistory(
        entryId,
        previousStatus,
        nextStatus
    );

    state = std::move(newState);

    const auto entry = repository.findById(entryId);

    if (!entry) {
        throw std::runtime_error(
            "LogEntry disappeared after status update."
        );
    }

    publisher.publish({
        LogEventType::StatusChanged,
        entryId,
        entry->level,
        entry->message,
        previousStatus + " -> " + nextStatus
        });

    if (nextStatus == "Опрацьований") {
        publisher.publish({
            LogEventType::ProcessingCompleted,
            entryId,
            entry->level,
            entry->message,
            "Опрацювання запису завершено."
            });
    }
}