
#include "Database.h"
#include "LogEntryRepository.h"
#include "LogEntryState.h"
#include "LogEventObserver.h"
#include "PriorityStrategy.h"

#include <gtest/gtest.h>
#include <memory>
#include <cmath>

// ================= STRATEGY =================

// Перевірка першого алгоритму.
TEST(StrategyTest, MaximumValue) {
    MaximumPriorityFactory factory;
    PriorityContext context(factory.createStrategy());

    EXPECT_DOUBLE_EQ(
        context.calculate({ 8.0, 6.0, 30.0 }), 8.0
    );
}

// Перевірка другого алгоритму.
TEST(StrategyTest, WeightedSum) {
    WeightedSumFactory factory;
    PriorityContext context(factory.createStrategy());

    EXPECT_NEAR(
        context.calculate({ 8.0, 6.0, 30.0 }), 6.8, 0.000001
    );
}

// Перевірка третього алгоритму.
TEST(StrategyTest, NormalizedWeightedSum) {
    NormalizedWeightedSumFactory factory;
    PriorityContext context(factory.createStrategy());

    EXPECT_NEAR(
        context.calculate({ 8.0, 6.0, 30.0 }), 0.68, 0.000001
    );
}

// ================= OBSERVER =================

class TestObserver : public ILogEventObserver {
public:
    int count = 0;

    void update(const LogEvent&) override {
        ++count;
    }
};

// Перевірка отримання події підписником.
TEST(ObserverTest, ReceivesEvent) {
    LogEventPublisher publisher;
    auto observer = std::make_shared<TestObserver>();

    publisher.subscribe(observer);

    publisher.publish({
        LogEventType::RecordReceived,
        1, "INFO", "Test", "New entry"
        });

    EXPECT_EQ(observer->count, 1);
}

// Перевірка відключення підписника.
TEST(ObserverTest, Unsubscribe) {
    LogEventPublisher publisher;
    auto observer = std::make_shared<TestObserver>();

    publisher.subscribe(observer);
    publisher.unsubscribe(observer);

    publisher.publish({
        LogEventType::RecordReceived,
        1, "INFO", "Test", "New entry"
        });

    EXPECT_EQ(observer->count, 0);
}

// ================= STATE =================

// Створення тестового запису в базі даних.
int createTestEntry() {
    static int sourceId = 0;

    if (sourceId == 0) {
        sourceId = static_cast<int>(
            getStorage().insert({
                0, "Test Source", "Test", "Localhost", true
                })
            );
    }

    LogEntry entry;
    entry.eventSourceId = sourceId;
    entry.timestamp = "2026-11-23 12:00:00";
    entry.level = "INFO";
    entry.message = "Test entry";
    entry.durationMs = 100;
    entry.importance = 8;
    entry.urgency = 6;
    entry.waitingTimeMinutes = 30;
    entry.status = "Новий";

    LogEntryRepository repository;
    return repository.add(entry);
}

// Перевірка дозволених переходів.
TEST(StateTest, ValidTransitions) {
    initializeDatabase();

    LogEntryRepository repository;
    LogEventPublisher publisher;

    int id = createTestEntry();

    LogEntryWorkflow workflow(id, repository, publisher);

    workflow.check();
    EXPECT_EQ(workflow.currentStatus(), "Перевірений");

    workflow.process();
    EXPECT_EQ(workflow.currentStatus(), "Опрацьований");
}

// Перевірка забороненого переходу.
TEST(StateTest, InvalidTransition) {
    initializeDatabase();

    LogEntryRepository repository;
    LogEventPublisher publisher;

    int id = createTestEntry();

    LogEntryWorkflow workflow(id, repository, publisher);

    EXPECT_THROW(
        workflow.process(),
        std::logic_error
    );

    EXPECT_EQ(workflow.currentStatus(), "Новий");
}
