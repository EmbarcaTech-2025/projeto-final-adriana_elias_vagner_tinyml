add_executable(${PROJECT_NAME}
    main.c
    i2c_config.c
    mpu.c
    leitura_sensor.c
    display_oled.c
    oled_display.c
    ssd1306_text.c
    #tarefa_oled_ax.c
    #tarefa_oled_ay.c
    #tarefa_oled_az.c
    #tarefa_oled_gx.c
    #tarefa_oled_gy.c
    #tarefa_oled_gz.c
    #tarefa_oled_temp.c
    tarefa_botao_a.c
    tarefa_calibracao.c
    numeros_display.c
    oled_animacoes.c
    numeros_medios.c
    digitos_medios_utils.c
    buffer_sensor.c
    mpu_calibracao.c
    oled_context.c
)

# Corrige a saída para build/ em vez de build/src/
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_sources(${PROJECT_NAME} PRIVATE
    ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../inc

)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    FreeRTOS-Kernel
    hardware_adc
    hardware_i2c
    hardware_gpio
)

pico_add_extra_outputs(${PROJECT_NAME})
