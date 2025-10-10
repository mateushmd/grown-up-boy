add_subdirectory(cartridge)
add_subdirectory(cartridge_factory)
add_subdirectory(cpu)
add_subdirectory(memory)
add_subdirectory(timer)

add_library(gameboy_lib STATIC
    gameboy.cpp
)

target_link_libraries(gameboy_lib PUBLIC
    cartridge_lib
    cartridge_factory_lib
    cpu_lib
    memory_lib
    timer_lib
)

target_include_directories(gameboy_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)
