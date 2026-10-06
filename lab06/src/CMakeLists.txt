# --- Файл: src/CMakeLists.txt ---

# 1. Автоматически ищем все .cpp и .hpp файлы во всех подпапках внутри src/
file(GLOB_RECURSE SRC "*.cpp" "*.hpp" "*.h")

# ВАЖНО: Исключаем main.cpp из библиотеки, так как точка входа (main) 
# должна быть только в самом приложении, а не в библиотеке.
list(REMOVE_ITEM SRC "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# 2. Создаем статическую библиотеку game_core из найденных файлов
add_library(game_core ${SRC})

# 3. Подключаем к нашей библиотеке SFML
target_link_libraries(game_core PRIVATE sfml-graphics sfml-window sfml-system)

# 4. Указываем библиотеке, где искать заголовочные файлы
target_include_directories(game_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})