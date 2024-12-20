# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(DIA)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем пути к исходникам и заголовкам
include_directories(include)
include_directories(src)



add_executable(DIA src/main.cpp)




