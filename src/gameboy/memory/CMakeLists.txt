add_library(memory_lib STATIC
    memory.cpp
)

target_include_directories(memory_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
