cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(mpw_bringup_rp_fw)
pico_sdk_init()

FILE(GLOB sources "*.c")
FILE(GLOB headers "*.h")

add_executable(mpw_bringup_rp_fw
    ${sources}
    ${headers}
)

# Pico library
target_link_libraries(mpw_bringup_rp_fw
    pico_stdlib
    pico_multicore
    hardware_i2c
)

target_include_directories(mpw_bringup_rp_fw PRIVATE .)

# Enable USB UART, disable UART pins
pico_enable_stdio_usb(mpw_bringup_rp_fw 1)
pico_enable_stdio_uart(mpw_bringup_rp_fw 1)

# Create programming files
pico_add_extra_outputs(mpw_bringup_rp_fw)
