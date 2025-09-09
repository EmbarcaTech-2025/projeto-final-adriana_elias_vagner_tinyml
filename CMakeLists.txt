cmake_minimum_required(VERSION 3.13.1)

set(PROJECT_NAME tinyML)

set(PROJECT_FOLDER .)

include(pico_sdk_import.cmake)

project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

pico_sdk_init()

# Set any variables required for importing FreeRTOS librarie
SET(FREERTOS_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS)
message("FreeRTOS Kernel located in ${FREERTOS_PATH}")

# include FreeRTOS cmake
include(${FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

add_definitions(-DEIDSP_QUANTIZE_FILTERBANK=0
                -DEI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN=0
                -DARM_MATH_LOOPUNROLL
)

add_executable(${PROJECT_NAME}
  src/main.cpp
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
  src/ml.cpp
)

include(${PROJECT_FOLDER}/edge-impulse-sdk/cmake/utils.cmake)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    FreeRTOS-Kernel-Heap4
    hardware_i2c
    hardware_pwm
    hardware_adc
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${PROJECT_FOLDER}/tflite-model
    ${PROJECT_FOLDER}/model-parameters
    ${PROJECT_FOLDER}
)

# enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

RECURSIVE_FIND_FILE(SOURCE_FILES "${PROJECT_FOLDER}/edge-impulse-sdk" "*.cpp")
RECURSIVE_FIND_FILE(MODEL_FILES "${PROJECT_FOLDER}/tflite-model" "*.cpp")
RECURSIVE_FIND_FILE(CC_FILES "${PROJECT_FOLDER}/edge-impulse-sdk" "*.cc")
RECURSIVE_FIND_FILE(C_FILES "${PROJECT_FOLDER}/edge-impulse-sdk" "*.c")

list(APPEND SOURCE_FILES ${S_FILES})
list(APPEND SOURCE_FILES ${C_FILES})
list(APPEND SOURCE_FILES ${CC_FILES})
list(APPEND SOURCE_FILES ${MODEL_FILES})

# add all sources to the project
target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES})

pico_add_extra_outputs(${PROJECT_NAME})
