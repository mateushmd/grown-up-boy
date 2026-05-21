add_library(timer STATIC timer.cpp)

target_include_directories(timer 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../.."
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)
