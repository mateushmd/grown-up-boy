add_library(time_lib STATIC
    time.cpp
)

target_include_directories(time_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
