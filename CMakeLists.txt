cmake_minimum_required(VERSION 3.17)
project(vaccine_shell C)

set(CMAKE_C_STANDARD 11)

include_directories(code)
include_directories(lib)

add_executable(vaccine_shell
        code/operations.c
        code/reader.c
        lib/operations.h
        lib/reader.h
        code/list.c
        lib/list.h
        main.c)
