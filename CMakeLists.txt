cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME tinyML)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJECT_NAME} C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set any variables required for importing FreeRTOS librarie
SET(FREERTOS_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS)
message("FreeRTOS Kernel located in ${FREERTOS_PATH}")

# include FreeRTOS cmake
include(${FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Add executable. Default name is the project name, version 0.1
add_executable(${PROJECT_NAME}
        main.c
        lib/ssd1306_i2c.c
        src/button_a.c
        src/button_b.c
        src/button_j.c
        src/ctrl.c
        src/display_gate.c
        src/led_rgb.c
        src/oled/display.c
        src/oled/i2c.c
        src/util.c
        src/mpu6500.c
  )


#add_library(mpu6500 STATIC
#       src/mpu6500.c
#)
#
# add_library(mpu6500 OBJECT
#     src/mpu6500.c
# )

pico_add_extra_outputs(${PROJECT_NAME})

# Add the standard library to the build
target_link_libraries(${PROJECT_NAME}
                                pico_stdlib
                                FreeRTOS-Kernel-Heap4
                                hardware_i2c
                                hardware_pwm
                                )

target_include_directories(${PROJECT_NAME} PRIVATE
   ${CMAKE_CURRENT_LIST_DIR}
   ${CMAKE_CURRENT_LIST_DIR}/include
   ${CMAKE_CURRENT_LIST_DIR}/src
)



pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
