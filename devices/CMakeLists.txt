add_library(devices STATIC window.cpp)

target_include_directories(devices
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_sources(devices
    PRIVATE
        window.cpp
)

target_link_libraries(devices PRIVATE SDL3::SDL3)