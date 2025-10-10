add_library(file_lib STATIC
    file.cpp
)

target_include_directories(file_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
