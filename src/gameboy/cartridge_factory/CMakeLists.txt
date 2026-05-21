add_library(cartridge_factory STATIC
    cartridge_factory.cpp
)

target_include_directories(cartridge_factory 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../.."
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" 
)
