add_subdirectory(bitwise)
add_subdirectory(file)
add_subdirectory(gameboy)
add_subdirectory(time)

add_executable(gub
    main.cpp
)

target_link_libraries(gub PRIVATE
    bitwise_mod
    file_mod
    gameboy_mod
    time_mod
)
