add_library(util)

target_include_directories(util
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_sources(util
    PRIVATE
        bitwise.cpp
        file.cpp
)