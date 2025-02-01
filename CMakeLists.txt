cmake_minimum_required(VERSION 3.15...3.30)

project(grown-up-boy VERSION 0.1 LANGUAGES CXX
        DESCRIPTION "A Game Boy emulator")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/_vendor")
set(FETCHCONTENT_BASE_DIR ${VENDOR_DIR})

option(USE_VENDORED "Use vendored libraries" ON)
option(USE_FETCH_CONTENT "Automatically fetch SDL3 library if vendored version is not found" ON)

set(VENDORED_SDL3_PATH "${VENDOR_DIR}/sdl3-build" CACHE PATH "Path to vendored SDL3")

if(USE_VENDORED)
    if(EXISTS ${VENDORED_SDL3_PATH})
        message(STATUS "Using vendored SDL3 from: ${VENDORED_SDL3_PATH}")
    elseif(USE_FETCH_CONTENT)
        message(STATUS "Vendored SDL3 not found, fetching SDL3 automatically using FetchContent...")
        include(FetchContent)

        FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG 535d80badefc83c5c527ec5748f2a20d6a9310fe 
            OVERRIDE_FIND_PACKAGE
        )
        FetchContent_MakeAvailable(SDL3)
    else()
        message(FATAL_ERROR "Vendored SDL3 not found at ${VENDORED_SDL3_PATH}. You can either specify a valid path with -VENDORED_SDL3_PATH or use -DUSE_FETCH_CONTENT=ON to automatically download SDL3.")
    endif()
    set(SDL3_DIR ${VENDORED_SDL3_PATH})
else()
    message(STATUS "Using pre-installed SDL3. Make sure it's available in the system or specified via -SDL3_DIR.")
endif()

if(NOT USE_VENDORED OR EXISTS ${VENDORED_SDL3_PATH})
    find_package(SDL3 CONFIG REQUIRED)
endif()

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_subdirectory(core ${CMAKE_CURRENT_BINARY_DIR}/core-build) 
add_subdirectory(devices ${CMAKE_CURRENT_BINARY_DIR}/devices-build)

add_executable(gub main/main.cpp)

target_link_libraries(gub PRIVATE core devices)
target_link_libraries(gub PRIVATE SDL3::SDL3)