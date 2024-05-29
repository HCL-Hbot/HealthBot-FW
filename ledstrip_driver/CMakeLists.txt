add_library(LEDSTRIP_DRIVER INTERFACE)

target_include_directories(LEDSTRIP_DRIVER INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(LEDSTRIP_DRIVER INTERFACE
    pico_stdlib
    PicoLed
)