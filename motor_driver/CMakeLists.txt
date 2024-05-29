add_library(STEPMOTOR_DRIVER INTERFACE)

target_include_directories(STEPMOTOR_DRIVER INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(STEPMOTOR_DRIVER INTERFACE
    pico_stdlib
    hardware_pwm
)