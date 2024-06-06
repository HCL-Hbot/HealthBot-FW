add_library(EXPANDER_DRIVER INTERFACE)

target_include_directories(EXPANDER_DRIVER INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(EXPANDER_DRIVER INTERFACE
    pico_stdlib
    hardware_i2c
)