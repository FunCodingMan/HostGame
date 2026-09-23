cmake_minimum_required(VERSION 3.8)
project(sfml-test)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2 COMPONENTS window graphics system REQUIRED)

add_executable(app src/main.cpp src/Core/Game.cpp src/Entities/Parasite.cpp)
target_include_directories(app PRIVATE include)
target_link_libraries(app PRIVATE sfml-graphics sfml-window sfml-system)