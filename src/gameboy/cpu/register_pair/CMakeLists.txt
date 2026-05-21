add_library(register_pair STATIC register_pair.cpp)

target_include_directories(register_pair PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/../../.."
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)
