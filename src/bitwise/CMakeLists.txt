add_library(bitwise_lib STATIC
    bitwise.cpp
)

target_include_directories(bitwise_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
