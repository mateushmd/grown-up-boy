add_library(cpu_lib STATIC
    cpu.cpp
)

target_include_directories(cpu_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
