cmake_minimum_required(VERSION 3.20)
project(ImplicitClient LANGUAGES CXX)

# Створюємо виконуваний файл
add_executable(ImplicitClient main.cpp)

# Вказуємо шлях до заголовочних файлів бібліотеки партнера
target_include_directories(ImplicitClient PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Підключаємо імпортну бібліотеку (.lib) для етапу лінкування
target_link_libraries(ImplicitClient PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/LinearSystems.lib
)

# Автоматично копіюємо .dll файл у папку з скомпільованим .exe після збірки
add_custom_command(TARGET ImplicitClient POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/bin/LinearSystems.dll"
    "$<TARGET_FILE_DIR:ImplicitClient>"
)