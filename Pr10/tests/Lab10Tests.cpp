#include "Database.h"
#include "LogEntryRepository.h"
#include "LogEntryState.h"
#include "LogEventObserver.h"
#include "PriorityStrategy.h"

#include <gtest/gtest.h>

#include <cmath>
#include <memory>
#include <stdexcept>

// =====================================================
// Допоміжний клас для перевірки Observer
// =====================================================

class CountingObserver : public ILogEventObserver {
public:
    int received = 0;
    int changed = 0;
    int completed = 0;

    void update(const LogEvent& event) override {
        switch (event.type) {
        case LogEventType::RecordReceived:
            ++received;
            break;

        case LogEventType::StatusChanged:
            ++changed;
            break;

        case LogEventType::ProcessingCompleted:
            ++completed;
            break;
        }
    }
};

// =====================================================
// ТЕСТИ STRATEGY ТА FACTORY METHOD
// =====================================================

class PriorityStrategyTest : public ::testing::Test {
protected:
    PriorityInput input{ 8.0, 6.0, 30.0 };
};

// Перевірка алгоритму максимального значення.
TEST_F(PriorityStrategyTest, MaximumStrategy) {
    MaximumPriorityFactory factory;

    PriorityContext context(factory.createStrategy());

    EXPECT_NEAR(context.calculate(input), 8.0, 0.000001);
}

// Перевірка алгоритму зваженої суми.
TEST_F(PriorityStrategyTest, WeightedSum) {
    WeightedSumFactory factory;

    PriorityContext context(factory.createStrategy());

    EXPECT_NEAR(context.calculate(input), 6.8, 0.000001);
}

// Перевірка нормованої зваженої суми.
TEST_F(PriorityStrategyTest, NormalizedWeightedSum) {
    NormalizedWeightedSumFactory factory;

    PriorityContext context(factory.createStrategy());

    EXPECT_NEAR(context.calculate(input), 0.68, 0.000001);
}

// Перевірка заміни стратегії під час виконання.
TEST_F(PriorityStrategyTest, StrategyReplacement) {
    PriorityContext context(
        std::make_unique<MaximumPriorityStrategy>()
    );

    EXPECT_NEAR(context.calculate(input), 8.0, 0.000001);

    context.setStrategy(
        std::make_unique<WeightedSumStrategy>()
    );

    EXPECT_NEAR(context.calculate(input), 6.8, 0.000001);
}

// Перевірка відхилення некоректних параметрів.
TEST_F(PriorityStrategyTest, RejectsInvalidInput) {
    MaximumPriorityStrategy strategy;

    EXPECT_THROW(
        strategy.calculate({ 15.0, 5.0, 20.0 }),
        std::invalid_argument
    );
}

// Перевірка відхилення нульового вказівника на стратегію.
TEST(PriorityStrategyTest, RejectsNullStrategy) {
    EXPECT_THROW(
        PriorityContext(nullptr),
        std::invalid_argument
    );
}

// =====================================================
// ТЕСТИ OBSERVER
// =====================================================

// Перевірка отримання події всіма підписниками.
TEST(ObserverTest, NotifiesAllSubscribers) {
    LogEventPublisher publisher;

    auto first = std::make_shared<CountingObserver>();
    auto second = std::make_shared<CountingObserver>();

    publisher.subscribe(first);
    publisher.subscribe(second);

    publisher.publish({
        LogEventType::RecordReceived,
        1,
        "INFO",
        "Test event",
        "New entry"
        });

    EXPECT_EQ(first->received, 1);
    EXPECT_EQ(second->received, 1);
}

// Перевірка відключення одного спостерігача.
TEST(ObserverTest, UnsubscribedObserverReceivesNoEvents) {
    LogEventPublisher publisher;

    auto first = std::make_shared<CountingObserver>();
    auto second = std::make_shared<CountingObserver>();

    publisher.subscribe(first);
    publisher.subscribe(second);

    publisher.unsubscribe(second);

    publisher.publish({
        LogEventType::RecordReceived,
        2,
        "INFO",
        "Another event",
        "New entry"
        });

    EXPECT_EQ(first->received, 1);
    EXPECT_EQ(second->received, 0);
}

// Перевірка відсутності повторної підписки.
TEST(ObserverTest, DoesNotSubscribeTwice) {
    LogEventPublisher publisher;

    auto observer = std::make_shared<CountingObserver>();

    publisher.subscribe(observer);
    publisher.subscribe(observer);

    publisher.publish({
        LogEventType::RecordReceived,
        3,
        "INFO",
        "Test",
        "New entry"
        });

    EXPECT_EQ(observer->received, 1);
}

// =====================================================
// ТЕСТИ STATE ТА REPOSITORY
// =====================================================

class LogEntryStateTest : public ::testing::Test {
protected:
    LogEntryRepository repository;

    int sourceId = 0;

    std::vector<int> entryIds;

    void SetUp() override {
        initializeDatabase();

        // Створюємо окреме тестове джерело.
        sourceId = static_cast<int>(
            getStorage().insert({
                0,
                "Lab10 Test Source",
                "Test",
                "Localhost",
                true
                })
            );
    }

    int createEntry() {
        LogEntry entry;

        entry.eventSourceId = sourceId;
        entry.timestamp = "2026-11-23 12:00:00";
        entry.level = "INFO";
        entry.message = "Google Test entry";
        entry.durationMs = 100;
        entry.importance = 8;
        entry.urgency = 6;
        entry.waitingTimeMinutes = 30;
        entry.status = "Новий";

        const int id = repository.add(entry);

        entryIds.push_back(id);

        return id;
    }

    void TearDown() override {
        using namespace sqlite_orm;

        auto& storage = getStorage();

        // Видаляємо тестову історію та записи.
        for (const int id : entryIds) {
            storage.remove_all<LogEntryStatusHistory>(
                where(
                    c(&LogEntryStatusHistory::logEntryId) == id
                )
            );

            storage.remove<LogEntry>(id);
        }

        if (sourceId != 0) {
            storage.remove<EventSource>(sourceId);
        }
    }
};

// Перевірка успішних переходів між станами.
TEST_F(LogEntryStateTest, ValidStateTransitions) {
    const int id = createEntry();

    LogEventPublisher publisher;

    LogEntryWorkflow workflow(
        id,
        repository,
        publisher
    );

    EXPECT_EQ(workflow.currentStatus(), "Новий");

    workflow.check();

    EXPECT_EQ(workflow.currentStatus(), "Перевірений");

    workflow.process();

    EXPECT_EQ(workflow.currentStatus(), "Опрацьований");
}

// Перевірка заборони недопустимих переходів.
TEST_F(LogEntryStateTest, RejectsInvalidTransition) {
    const int id = createEntry();

    LogEventPublisher publisher;

    LogEntryWorkflow workflow(
        id,
        repository,
        publisher
    );

    // Новий запис не можна одразу опрацювати.
    EXPECT_THROW(
        workflow.process(),
        std::logic_error
    );

    EXPECT_EQ(workflow.currentStatus(), "Новий");

    const auto history = repository.findStatusHistory(id);

    EXPECT_TRUE(history.empty());
}

// Перевірка збереження історії зміни станів.
TEST_F(LogEntryStateTest, SavesStatusHistory) {
    const int id = createEntry();

    LogEventPublisher publisher;

    LogEntryWorkflow workflow(
        id,
        repository,
        publisher
    );

    workflow.check();
    workflow.process();

    const auto history = repository.findStatusHistory(id);

    ASSERT_EQ(history.size(), 2);

    EXPECT_EQ(history[0].previousStatus, "Новий");
    EXPECT_EQ(history[0].newStatus, "Перевірений");

    EXPECT_EQ(history[1].previousStatus, "Перевірений");
    EXPECT_EQ(history[1].newStatus, "Опрацьований");
}

// Перевірка відновлення поточного стану із БД.
TEST_F(LogEntryStateTest, RestoresStateFromDatabase) {
    const int id = createEntry();

    LogEventPublisher publisher;

    {
        LogEntryWorkflow workflow(
            id,
            repository,
            publisher
        );

        workflow.check();
    }

    // Створюємо новий контекст для того самого запису.
    LogEntryWorkflow restoredWorkflow(
        id,
        repository,
        publisher
    );

    EXPECT_EQ(
        restoredWorkflow.currentStatus(),
        "Перевірений"
    );
}

// Перевірка відхилення запису.
TEST_F(LogEntryStateTest, CanRejectNewEntry) {
    const int id = createEntry();

    LogEventPublisher publisher;

    LogEntryWorkflow workflow(
        id,
        repository,
        publisher
    );

    workflow.reject();

    EXPECT_EQ(workflow.currentStatus(), "Відхилений");

    const auto history = repository.findStatusHistory(id);

    ASSERT_EQ(history.size(), 1);

    EXPECT_EQ(history[0].newStatus, "Відхилений");
}

// Перевірка передачі подій спостерігачам під час зміни стану.
TEST_F(LogEntryStateTest, PublishesEventsAfterTransition) {
    const int id = createEntry();

    LogEventPublisher publisher;

    auto observer = std::make_shared<CountingObserver>();

    publisher.subscribe(observer);

    LogEntryWorkflow workflow(
        id,
        repository,
        publisher
    );

    workflow.check();
    workflow.process();

    EXPECT_EQ(observer->changed, 2);
    EXPECT_EQ(observer->completed, 1);
}

// Перевірка відсутності змін у БД при невідповідності очікуваного стану.
TEST_F(LogEntryStateTest, KeepsDatabaseConsistentOnFailedTransition) {
    const int id = createEntry();

    EXPECT_THROW(
        repository.changeStatusWithHistory(
            id,
            "Перевірений",
            "Опрацьований"
        ),
        std::runtime_error
    );

    const auto entry = repository.findById(id);

    ASSERT_TRUE(entry.has_value());

    EXPECT_EQ(entry->status, "Новий");

    const auto history = repository.findStatusHistory(id);

    EXPECT_TRUE(history.empty());
}