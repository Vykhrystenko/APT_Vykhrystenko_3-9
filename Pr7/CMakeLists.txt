cmake_minimum_required(VERSION 3.21)

project(MatrixLibLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Основна статична бібліотека
add_library(MatrixLib STATIC
    src/MatrixLib.cpp
)

target_include_directories(MatrixLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# Демонстраційна програма
add_executable(MatrixDemo
    demo/main.cpp
)

target_link_libraries(MatrixDemo
    PRIVATE MatrixLib
)


# Демонстрація антипатернів
add_executable(AntipatternDemo
    antipattern/main.cpp
)


# Google Test
find_package(GTest CONFIG REQUIRED)

enable_testing()

add_executable(MatrixTests
    tests/MatrixTests.cpp
)

target_link_libraries(MatrixTests
    PRIVATE
        MatrixLib
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(MatrixTests)