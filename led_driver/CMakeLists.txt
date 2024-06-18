add_library(LED_DRIVER INTERFACE)

target_include_directories(LED_DRIVER INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(LED_DRIVER INTERFACE
    pico_stdlib
    PicoLed

    EXPANDER_DRIVER
)