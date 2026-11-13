#include "Database.h"

void initializeDatabase() {
    getStorage().sync_schema();
}
