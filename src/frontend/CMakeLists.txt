add_executable(gub
    main.cpp
)

target_link_libraries(gub
    PRIVATE
    gub_common
    gub_core
    SDL3::SDL3
)
