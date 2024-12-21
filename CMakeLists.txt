# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(DIA)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Создаем библиотеку external как INTERFACE
add_library(external INTERFACE)

# Находим библиотеку Boost
find_package(Boost 1.70 REQUIRED COMPONENTS system)

# Находим библиотеку nlohmann_json (если установлена через apt)
find_package(nlohmann_json REQUIRED)

# Указываем пути для заголовочных файлов в библиотеке external
target_include_directories(external INTERFACE
    ${Boost_INCLUDE_DIRS}        # Пути для Boost
    ${NLOHMANN_JSON_INCLUDE_DIRS} # Пути для nlohmann/json
)

# Указываем пути для заголовочных файлов в основном проекте
include_directories(include)
include_directories(src)

add_library(my_classes STATIC src/client.cpp src/client.cpp)


# Добавляем исполняемый файл SERVER
add_executable(SERVER src/server_main.cpp)
target_link_libraries(SERVER external)  # Подключаем external
target_link_libraries(SERVER my_classes)  

# Добавляем исполняемый файл CLIENT
add_executable(CLIENT src/client_main.cpp)
target_link_libraries(CLIENT external)  # Подключаем external
target_link_libraries(CLIENT my_classes) 

add_executable(TEST src/test.cpp)
target_link_libraries(TEST external)  # Подключаем external
target_link_libraries(TEST my_classes)  