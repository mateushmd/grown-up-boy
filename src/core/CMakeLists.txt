add_library(gub_core STATIC
    src/audio.cpp
    src/bus.cpp
    src/cartridge.cpp
    src/cpu.cpp
    src/gb.cpp
    src/joypad.cpp
    src/lcd.cpp
    src/sync.cpp
    src/timer.cpp
)

target_link_libraries(gub_core
    PRIVATE
    gub_common
)

target_include_directories(gub_core
    PUBLIC
    $
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(MSVC)
    target_compile_options(gub_core PRIVATE /W4)
else()
    target_compile_options(gub_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
