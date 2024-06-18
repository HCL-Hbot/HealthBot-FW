cmake_minimum_required(VERSION 3.18)
set(PICO_BOARD pico_w)
set(PICO_SDK_FETCH_FROM_GIT ON)
set(PROJECT_NAME HBOT-FW-SB)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

#=== START ===# Automatical fetching of libraries & dependencies:
include(FetchContent)

# FREE RTOS
FetchContent_Declare(
  FreeRTOS
  GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel
  GIT_TAG V11.0.1
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS
)
# LVGL
FetchContent_Declare(
  lvgl
  GIT_REPOSITORY https://github.com/lvgl/lvgl
  GIT_TAG release/v8.3
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/lvgl
)
# LVGL Porting Drivers, customized for HBOT FW (INHOUSE REPO)
FetchContent_Declare(
  lv_drivers
  GIT_REPOSITORY https://github.com/HCL-Hbot/lv_drivers
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/lv_drivers
)
# EMBEDDED CLI
FetchContent_Declare(
  embedded_cli
  GIT_REPOSITORY https://github.com/funbiscuit/embedded-cli.git
  GIT_TAG master
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/embedded-cli
)
# PicoLED
FetchContent_Declare(
  PicoLED
  GIT_REPOSITORY https://github.com/HCL-Hbot/EXLIB-PicoLED.git
  GIT_TAG master
  SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/PicoLED
)
# Fetch and make available dependencies
FetchContent_MakeAvailable(embedded_cli PicoLED)
#=== END ===# Automatical fetching of libraries & dependencies:

project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif ()

# Initialize the SDK
pico_sdk_init()
include(${CMAKE_CURRENT_LIST_DIR}/lib/PicoLED/PicoLed.cmake)

add_compile_options(-Wall
  -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
  -Wno-unused-function # we have some for the docs that aren't called
  -Wno-maybe-uninitialized
)

if (PICO_CYW43_SUPPORTED)
  if (NOT TARGET pico_cyw43_arch)
    message("Skipping build as support is not available")
  else ()
    add_executable(${PROJECT_NAME} "src/main.cpp")

    # Enable USB output, disable UART output
    pico_enable_stdio_usb(${PROJECT_NAME} 1)
    pico_enable_stdio_uart(${PROJECT_NAME} 0)

    target_include_directories(${PROJECT_NAME} PUBLIC
      ${CMAKE_CURRENT_LIST_DIR}/src
      ${CMAKE_CURRENT_LIST_DIR}/motor_driver
      ${CMAKE_CURRENT_LIST_DIR}/display
      ${CMAKE_CURRENT_LIST_DIR}/async_uart
      ${CMAKE_CURRENT_LIST_DIR}/ledstrip_driver
      ${CMAKE_CURRENT_LIST_DIR}/config
      ${CMAKE_CURRENT_LIST_DIR}/brainboard_com
      ${CMAKE_CURRENT_LIST_DIR}/radar
      ${CMAKE_CURRENT_LIST_DIR}/io_expander
      ${CMAKE_CURRENT_LIST_DIR}/led_driver
    )

    # Select the heap port.  values between 1-4 will pick a heap.
    set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

    # Select the native compile PORT
    set(FREERTOS_PORT "GCC_ARM_CM0" CACHE STRING "" FORCE)
    add_library(freertos_config INTERFACE)
    FetchContent_MakeAvailable(FreeRTOS)

    add_library(lv_driver_hooks "src/lvgl_hooks/lvgl_hooks.c" "src/lvgl_hooks/lv_port_disp.c")
    target_include_directories(lv_driver_hooks PUBLIC "src/lvgl_hooks/" "config/")
    target_link_libraries(lv_driver_hooks pico_stdlib hardware_spi freertos_kernel lvgl::lvgl lv_drivers)

    # Specify path to own LVGL config header
    set(LV_CONF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/config/lv_conf.h CACHE STRING "" FORCE)
    FetchContent_MakeAvailable(lvgl)
    FetchContent_MakeAvailable(lv_drivers)
    target_include_directories(lv_drivers PUBLIC "config/")

    target_compile_definitions(lv_drivers PUBLIC "LV_CONF_INCLUDE_SIMPLE" "LV_LVGL_H_INCLUDE_SIMPLE")
    target_link_libraries(lv_drivers PUBLIC lv_driver_hooks)

    target_compile_definitions(lvgl PUBLIC "LV_CONF_INCLUDE_SIMPLE")
    target_include_directories(freertos_config INTERFACE "config/")

    # UI Library setup
    file(GLOB_RECURSE UI_SRC_FILES src/ui/*.c src/ui/*.h)
    add_library(ui ${UI_SRC_FILES})
    target_include_directories(ui PUBLIC src/ui)
    target_link_libraries(ui lvgl)

    add_library(cli_bindings STATIC src/cli_hooks/cli_methods.cpp src/cli_hooks/cli_port.cpp)
    target_include_directories(cli_bindings PUBLIC src/cli_hooks)
    target_link_libraries(cli_bindings PUBLIC embedded_cli_lib ui STEPMOTOR_DRIVER)

    target_link_libraries(${PROJECT_NAME} PUBLIC
        cli_bindings
        embedded_cli_lib

        DISPLAY
        ASYNC_UART
        LEDSTRIP_DRIVER
        STEPMOTOR_DRIVER
        BGT60LTR11XXX
        EXPANDER_DRIVER
        BRAINBOARD_COM
        LED_DRIVER

        pico_cyw43_arch_lwip_poll
        pico_stdlib
        pico_lwip_mbedtls
        pico_mbedtls
        hardware_spi
        freertos_kernel
        freertos_config
        lvgl::lvgl
        lvgl::examples
        lvgl::demos
        lv_drivers
        lv_driver_hooks
        ui
        PicoLed
    )
    pico_add_extra_outputs(${PROJECT_NAME})
  endif ()
endif()

add_subdirectory(display)
add_subdirectory(motor_driver)
add_subdirectory(async_uart)
add_subdirectory(ledstrip_driver)
add_subdirectory(brainboard_com)
add_subdirectory(radar)
add_subdirectory(io_expander)
add_subdirectory(led_driver)