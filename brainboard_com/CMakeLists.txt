add_library(BRAINBOARD_COM INTERFACE)

target_include_directories(BRAINBOARD_COM INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(BRAINBOARD_COM INTERFACE
    pico_stdlib
    ASYNC_UART
    STEPMOTOR_DRIVER
)