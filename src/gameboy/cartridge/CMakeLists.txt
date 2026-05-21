add_library(cartridge STATIC cartridge.cpp)

target_include_directories(cartridge 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../.."
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)
