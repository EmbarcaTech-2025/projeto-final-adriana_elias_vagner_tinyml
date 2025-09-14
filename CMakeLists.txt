cmake_minimum_required(VERSION 3.13.1)

set(PROJECT_NAME tinyML)

set(PROJECT_FOLDER .)

set(PICO_BOARD pico_w CACHE STRING "Board type")

#include(pico_sdk_import.cmake)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

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
                -DPICO_CYW43_ARCH_RPI_W

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
  src/mqtt_task.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/paho.mqtt.embedded-c/MQTTClient-C/src/MQTTClient.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/paho.mqtt.embedded-c/MQTTClient-C/src/MQTTConnectClient.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/paho.mqtt.embedded-c/MQTTClient-C/src/MQTTDeserializePublish.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/paho.mqtt.embedded-c/MQTTClient-C/src/MQTTSerializePublish.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/paho.mqtt.embedded-c/MQTTClient-C/src/MQTTSubscribeClient.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/paho.mqtt.embedded-c/MQTTClient-C/src/MQTTUnsubscribeClient.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/paho.mqtt.embedded-c/MQTTClient-C/src/transport.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/paho.mqtt.embedded-c/MQTTPacket/src/MQTTFormat.c
  ${CMAKE_CURRENT_LIST_DIR}/lib/paho.mqtt.embedded-c/MQTTPacket/src/MQTTPacket.c
  src/paho_network.c
  src/paho_timer.c
)

include(${PROJECT_FOLDER}/edge-impulse-sdk/cmake/utils.cmake)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    FreeRTOS-Kernel-Heap4
    hardware_i2c
    hardware_pwm
    hardware_adc
    pico_cyw43_arch_sys_freertos
    pico_lwip_freertos
    pico_lwip
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${PROJECT_FOLDER}/tflite-model
    ${PROJECT_FOLDER}/model-parameters
    ${PROJECT_FOLDER}
    ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_arch/include
    ${PICO_SDK_PATH}/lib/cyw43-driver/src
    ${PICO_SDK_PATH}/lib/cyw43-driver/include/
    ${PICO_SDK_PATH}/src/rp2_common/pico_cyw43_driver/include/
    ${PICO_SDK_PATH}/lib/lwip/src/include/
    ${PICO_SDK_PATH}/src/rp2_common/pico_lwip/include/
    ${PICO_SDK_PATH}/src/rp2_common/pico_rand/include/
    ${PICO_SDK_PATH}/src/rp2_common/pico_async_context/include/
    ${PICO_SDK_PATH}/lib/lwip/contrib/ports/freertos/include/
    ${CMAKE_CURRENT_LIST_DIR}/lib/paho.mqtt.embedded-c/MQTTClient-C/src/
    ${CMAKE_CURRENT_LIST_DIR}/lib/paho.mqtt.embedded-c/MQTTPacket/src/
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
