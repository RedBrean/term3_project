# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(DIA)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Находим библиотеку Boost
find_package(Boost 1.70 REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

# Указываем пути к заголовкам
include_directories(include)
include_directories(src)

# Добавляем исполняемый файл DIA
# add_executable(DIA src/main.cpp)
# target_link_libraries(DIA ${Boost_LIBRARIES})

# Добавляем исполняемый файл SERVER
add_executable(SERVER src/server.cpp)
target_link_libraries(SERVER ${Boost_LIBRARIES})  # Boost нужен серверу напрямую

# Добавляем исполняемый файл CLIENT
add_executable(CLIENT src/client.cpp)
target_link_libraries(CLIENT ${Boost_LIBRARIES})  # Boost нужен клиенту напрямую
