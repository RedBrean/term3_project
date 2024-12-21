# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(DIA)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Находим библиотеку Boost
find_package(Boost 1.70 REQUIRED COMPONENTS system)

# Находим библиотеку nlohmann_json (если установлена через apt)
find_package(nlohmann_json REQUIRED)

# Создаем библиотеку external как INTERFACE
add_library(external INTERFACE)

# Указываем пути для заголовочных файлов в библиотеке external
target_include_directories(external INTERFACE
    ${Boost_INCLUDE_DIRS}        # Пути для Boost
    ${NLOHMANN_JSON_INCLUDE_DIRS} # Пути для nlohmann/json
)

# Указываем пути для заголовочных файлов в основном проекте
include_directories(include)
include_directories(src)

# Добавляем исполняемый файл SERVER
add_executable(SERVER src/server.cpp)
target_link_libraries(SERVER external)  # Подключаем external

# Добавляем исполняемый файл CLIENT
add_executable(CLIENT src/client.cpp)
target_link_libraries(CLIENT external)  # Подключаем external

add_executable(TEST src/main.cpp)
target_link_libraries(TEST external)  # Подключаем external
