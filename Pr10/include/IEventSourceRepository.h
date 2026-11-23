#pragma once

#include "EventModels.h"

#include <optional>
#include <vector>

// Абстрактний інтерфейс доступу до основної сутності EventSource.
class IEventSourceRepository {
public:
    virtual ~IEventSourceRepository() = default;

    virtual int add(const EventSource& source) = 0;
    virtual std::optional<EventSource> findById(int id) const = 0;
    virtual std::vector<EventSource> findAll() const = 0;
    virtual void update(const EventSource& source) = 0;
    virtual void remove(int id) = 0;
};
