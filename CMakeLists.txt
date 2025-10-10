cmake_minimum_required(VERSION 3.15...3.30)

project(grown-up-boy VERSION 0.1 LANGUAGES CXX
        DESCRIPTION "A Game Boy emulator")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_subdirectory(src) 
