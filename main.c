/**
 * @file    main.c
 * @author  Ricardo e Vagner
 * @brief 
 * @version 0.1
 * @date    2025-06-25
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

#include "include/button_a.h"
#include "include/button_b.h"
#include "include/button_j.h"
#include "include/config.h"
#include "include/ctrl.h"
#include "include/display_gate.h"
#include "include/led_rgb.h"

/**
 * @brief Inicializa programa
 * - inicializa elementos de hardware que não fazem parte de tasks
 * - cria e inicializa semáforos
 * - cria tasks
 * - inicializa scheduler
 * 
 * @return int 
 */
int main()
{
    TaskHandle_t task_handle_button_a;
    TaskHandle_t task_handle_button_b;
    TaskHandle_t task_handle_button_j;
    TaskHandle_t task_handle_display_gate;

    stdio_init_all();

#ifdef DEBUG_BITDOG_LAB
    // Configuração utilizada para configurar a UART da BidDogLab para Debug
    gpio_set_function( 0, GPIO_FUNC_SIO);
    gpio_set_function( 1, GPIO_FUNC_SIO);
    gpio_set_function(17, GPIO_FUNC_UART);
    gpio_set_function(16, GPIO_FUNC_UART);
#endif

    printf("embarcatech-freertos-tarefa-2\n");

    ctrl_init();
    led_rgb_init();
    
    printf("Create all tasks\n");
    xTaskCreate(button_a_task,     "ButtonA_Task",     256, NULL, 1, &task_handle_button_a);
    xTaskCreate(button_b_task,     "ButtonB_Task",     256, NULL, 1, &task_handle_button_b);
    xTaskCreate(button_j_task,     "ButtonJ_Task",     256, NULL, 1, &task_handle_button_j);
    xTaskCreate(display_gate_task, "DisplayGate_Task", 256, NULL, 1, &task_handle_display_gate);

    printf("Start Scheduler\n");
    vTaskStartScheduler();

    printf("After Start Scheduler, never arrives this line\n");
    while (true) { }

    return 0;
}
