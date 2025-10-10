add_library(cartridge_lib STATIC
    cartridge.cpp
)

target_include_directories(cartridge_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
