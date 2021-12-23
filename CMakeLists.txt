cmake_minimum_required(VERSION 3.20)
project(calculator C)

set(CMAKE_C_STANDARD 99)
include_directories(/usr/include/mysql/)
include_directories(/usr/include/)
add_executable(calculator main.c)
target_link_libraries(calculator mysqlclient)
target_link_libraries(calculator pthread)