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

find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3-shared)

add_subdirectory(core ${CMAKE_CURRENT_BINARY_DIR}/core-build) 
add_subdirectory(devices ${CMAKE_CURRENT_BINARY_DIR}/devices-build)
add_subdirectory(util ${CMAKE_CURRENT_BINARY_DIR}/util-build)

add_executable(gub main/main.cpp)

target_link_libraries(gub PRIVATE util core devices)
target_link_libraries(gub PRIVATE SDL3::SDL3)

if(WIN32)
    add_custom_command(
        TARGET gub POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:gub>
        VERBATIM
    )
endif()