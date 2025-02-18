add_library(core)

target_include_directories(core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components/bus>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components/cpu>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/components/cartridge>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/debug>
)

target_sources(core
    PRIVATE
        components/bus/bus.cpp
        components/cpu/registers.cpp
        components/cpu/cpu.cpp
        components/cpu/instructions.cpp
        components/cartridge/cartridge.cpp
        components/cartridge/cartridge_factory.cpp
        debug/debugger.cpp
        debug/memory_inspector.cpp
)

target_link_libraries(core PUBLIC util)