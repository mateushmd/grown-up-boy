add_subdirectory(cartridge)
add_subdirectory(cartridge_factory)
add_subdirectory(cpu)
add_subdirectory(memory)
add_subdirectory(timer)

add_library(gameboy STATIC gameboy.cpp)

target_link_libraries(gameboy PUBLIC
    cartridge
    cartridge_factory
    cpu
    memory
    timer
)

target_include_directories(gameboy 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)
