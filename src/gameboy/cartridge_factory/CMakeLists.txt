add_library(cartridge_factory_lib STATIC
    cartridge_factory.cpp
)

target_include_directories(cartridge_factory_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
