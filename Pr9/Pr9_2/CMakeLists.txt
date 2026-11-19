cmake_minimum_required(VERSION 3.21)
project(Pr9_2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Спільні файли логіки (без main.cpp і без тестів)
set(CORE_SOURCES
    source/dtofraction.cpp
    source/fraction.cpp
    source/fracrcomparison.cpp
)

# 1. Основна демонстраційна програма
add_executable(${PROJECT_NAME}
    main.cpp
    ${CORE_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 2. Google Test
find_package(GTest CONFIG REQUIRED)
enable_testing()

add_executable(Pr9_2_Tests
    tests/dToFractionTests.cpp
    ${CORE_SOURCES}
)

target_include_directories(Pr9_2_Tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Pr9_2_Tests PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Pr9_2_Tests)