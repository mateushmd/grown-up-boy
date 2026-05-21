add_subdirectory(register_pair)

add_library(cpu STATIC cpu.cpp)

target_link_libraries(cpu PUBLIC
    register_pair
)

target_include_directories(cpu 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../.."
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)
