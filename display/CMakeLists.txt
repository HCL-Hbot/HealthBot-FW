add_library(DISPLAY INTERFACE)

target_include_directories(DISPLAY INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(DISPLAY INTERFACE
    pico_stdlib
    BRAINBOARD_COM
)