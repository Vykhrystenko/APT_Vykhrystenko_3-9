# Лабораторна робота №8 — Варіант 1

Консольний застосунок C++20 для системи обробки журналів подій. Використовує SQLite, sqlite_orm, Repository, SQLite C API (для порівняльної частини), CMake та vcpkg.

## Структура

```text
EventLog_Lab8_Variant1/
├── CMakeLists.txt
├── vcpkg.json
├── README.md
├── include/
│   ├── EventModels.h
│   ├── Database.h
│   ├── IEventSourceRepository.h
│   ├── SQLiteEventSourceRepository.h
│   ├── LogEntryRepository.h
│   └── RawSqliteRepository.h
└── src/
    ├── main.cpp
    ├── Database.cpp
    ├── SQLiteEventSourceRepository.cpp
    ├── LogEntryRepository.cpp
    └── RawSqliteRepository.cpp
```

## Залежності

У `vcpkg.json` оголошено порти `sqlite3` та `sqlite-orm`. vcpkg у manifest mode встановлює їх під час конфігурації CMake.

## Збирання у Windows

У прикладі нижче припускається, що vcpkg розміщено в `C:\vcpkg`. Замініть цей шлях на фактичний шлях до свого vcpkg.

```powershell
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
.\build\Release\EventLogLab8.exe
```

Для Visual Studio можна також відкрити папку проєкту через **File → Open → Folder**, якщо CMake та vcpkg налаштовані у середовищі.

## Що демонструє застосунок

1. Створення схеми SQLite через `sync_schema()`.
2. Додавання й отримання джерела подій через SQLite C API з prepared statements.
3. Ті самі операції через sqlite_orm та Repository.
4. CRUD основної сутності через інтерфейс `IEventSourceRepository`.
5. Фільтрацію, сортування, LIMIT, COUNT, SUM, AVG, GROUP BY та INNER JOIN.
6. Успішну транзакцію, відкат при порушенні зовнішнього ключа та часткове збереження без явної транзакції.
7. Підготовку п'яти джерел і п'ятнадцяти записів журналу на кожному запуску.

Файл `event_logs.db` створюється у робочій директорії програми. Перед завантаженням тестового набору дочірні записи видаляються першими, а потім видаляються джерела, тому демонстраційні дані не накопичуються між запусками.
