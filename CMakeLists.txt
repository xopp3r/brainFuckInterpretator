cmake_minimum_required(VERSION 3.28)

set (CMAKE_CXX_STANDARD 23)

project(BFInterpreter)


# Автоматически собрать все .cpp файлы из папки src/
file(GLOB SOURCES "src/*.cpp")

# Собрать исполняемый файл
add_executable(main ${SOURCES})