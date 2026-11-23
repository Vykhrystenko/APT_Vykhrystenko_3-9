#include "SQLiteEventSourceRepository.h"
#include "Database.h"
#include <stdexcept>

//Add a new EventSource to the database and return its id.
int SQLiteEventSourceRepository::add(const EventSource& source) {
    return static_cast<int>(getStorage().insert(source));
}
//Find an EventSource by its id. Returns std::nullopt if not found.
std::optional<EventSource> SQLiteEventSourceRepository::findById(int id) const {
    return getStorage().get_optional<EventSource>(id);
}
//Retrieve all EventSource records from the database.
std::vector<EventSource> SQLiteEventSourceRepository::findAll() const {
    return getStorage().get_all<EventSource>();
}
//Update an existing EventSource in the database. Throws an exception if the EventSource does not exist.
void SQLiteEventSourceRepository::update(const EventSource& source) {
    if (!getStorage().get_optional<EventSource>(source.id)) {
        throw std::runtime_error("Cannot update: EventSource with this id does not exist.");
    }

    getStorage().update(source);
}
//Remove an EventSource from the database by its id. Throws an exception if the EventSource does not exist.
void SQLiteEventSourceRepository::remove(int id) {
    if (!getStorage().get_optional<EventSource>(id)) {
        throw std::runtime_error("Cannot remove: EventSource with this id does not exist.");
    }
	// Remove the EventSource from the database.
    getStorage().remove<EventSource>(id);
}
