#pragma once

#include "IEventSourceRepository.h"

// A repository class that implements the IEventSourceRepository interface using SQLite for data storage.
class SQLiteEventSourceRepository final : public IEventSourceRepository {
public:
    int add(const EventSource& source) override;
    std::optional<EventSource> findById(int id) const override;
    std::vector<EventSource> findAll() const override;
    void update(const EventSource& source) override;
    void remove(int id) override;
};
