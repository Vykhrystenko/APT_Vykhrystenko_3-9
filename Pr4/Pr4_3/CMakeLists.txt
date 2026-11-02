cmake_minimum_required(VERSION 3.20)
project(ExplicitClientApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ExplicitClientApp main.cpp)

# Копіюємо LinearSystems.dll у папку з готовим .exe після збірки 
add_custom_command(TARGET ExplicitClientApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/bin/LinearSystems.dll"
    "$<TARGET_FILE_DIR:ExplicitClientApp>"
)