cmake_minimum_required(VERSION 3.20)

project(EventLogLab10 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SQLite3 REQUIRED)
find_package(SqliteOrm CONFIG REQUIRED)

add_library(EventLogCore STATIC
    src/Database.cpp
    src/SQLiteEventSourceRepository.cpp
    src/LogEntryRepository.cpp
    src/RawSqliteRepository.cpp
    src/LogEntryState.cpp
    src/PriorityStrategy.cpp
    src/LogEventObserver.cpp
)

target_include_directories(EventLogCore PUBLIC include)

target_link_libraries(EventLogCore
    PUBLIC
        sqlite_orm::sqlite_orm
        SQLite::SQLite3
)

add_executable(EventLogLab10
    src/main.cpp
)

target_link_libraries(EventLogLab10
    PRIVATE EventLogCore
)

include(CTest)

if(BUILD_TESTING)

    find_package(GTest CONFIG REQUIRED)

    include(GoogleTest)

    add_executable(Lab10Tests
        tests/Lab10Tests.cpp
    )

    target_link_libraries(Lab10Tests
        PRIVATE
            EventLogCore
            GTest::gtest_main
    )

    gtest_discover_tests(Lab10Tests)

    set_property(
        TARGET Lab10Tests
        PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}"
    )

endif()

set_property(
    TARGET EventLogLab10
    PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

if(MSVC)

    target_compile_options(EventLogCore PRIVATE /W4 /utf-8)
    target_compile_options(EventLogLab10 PRIVATE /W4 /utf-8)

    if(BUILD_TESTING)
        target_compile_options(Lab10Tests PRIVATE /W4 /utf-8)
    endif()

else()

    target_compile_options(EventLogCore PRIVATE
        -Wall -Wextra -Wpedantic
    )

    target_compile_options(EventLogLab10 PRIVATE
        -Wall -Wextra -Wpedantic
    )

    if(BUILD_TESTING)
        target_compile_options(Lab10Tests PRIVATE
            -Wall -Wextra -Wpedantic
        )
    endif()

endif()