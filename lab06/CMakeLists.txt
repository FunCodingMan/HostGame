# --- Файл: CMakeLists.txt (Корневой) ---

cmake_minimum_required(VERSION 3.8)
project(TheHost)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2 COMPONENTS window graphics system REQUIRED)

# 1. Подключаем папку src (CMake зайдет туда и выполнит вложенный CMakeLists.txt)
add_subdirectory(src)

# 2. Создаем исполняемый файл приложения ТОЛЬКО из main.cpp
add_executable(app src/main.cpp)

# 3. Линкуем к нашему приложению нашу же библиотеку game_core и SFML
target_link_libraries(app PRIVATE game_core sfml-graphics sfml-window sfml-system)

# 4. Чтобы main.cpp мог подключать заголовочные файлы
target_include_directories(app PRIVATE src)