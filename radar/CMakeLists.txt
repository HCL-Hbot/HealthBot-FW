add_library(BGT60LTR11XXX STATIC
    src/BGT60LTR11XXX_driver.cpp
    src/radar_task.cpp
)

target_include_directories(BGT60LTR11XXX PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/../lib/FreeRTOS-Kernel/include
)

target_link_libraries(BGT60LTR11XXX PUBLIC   
    pico_stdlib  
    hardware_spi
    freertos_kernel
    freertos_config
    BRAINBOARD_COM
)