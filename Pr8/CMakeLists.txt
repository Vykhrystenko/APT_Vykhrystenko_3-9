cmake_minimum_required(VERSION 3.20)

project(EventLogLab8 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SQLite3 REQUIRED)
find_package(SqliteOrm CONFIG REQUIRED)

add_executable(EventLogLab8
    src/main.cpp
    src/Database.cpp
    src/SQLiteEventSourceRepository.cpp
    src/LogEntryRepository.cpp
    src/RawSqliteRepository.cpp
)

target_include_directories(EventLogLab8 PRIVATE include)

target_link_libraries(EventLogLab8
    PRIVATE
        sqlite_orm::sqlite_orm
        SQLite::SQLite3
)

if(MSVC)
    target_compile_options(EventLogLab8 PRIVATE /W4 /utf-8)
else()
    target_compile_options(EventLogLab8 PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Running from the project directory keeps event_logs.db in a predictable place in Visual Studio.
set_property(TARGET EventLogLab8 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
