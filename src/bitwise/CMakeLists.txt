add_library(bitwise STATIC bitwise.cpp)

target_include_directories(bitwise 
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
)
