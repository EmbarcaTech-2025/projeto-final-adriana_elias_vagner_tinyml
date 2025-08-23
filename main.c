/**
 * @file    main.c
 * @author  grupo
 * @brief
 * @version 0.1
 * @date
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
#include "include/mpu6500.h"

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
    TaskHandle_t task_handle_mpu6500;

    stdio_init_all();

    printf("Embarcatech Fase-2 :: Projeto Final\n");

    ctrl_init();
    led_rgb_init();
    mpu6500_init(); // Inicializa o acelerômetro

    printf(":: Criando as tarefas ::\n");
    xTaskCreate(button_a_task,     "ButtonA_Task",     256, NULL, 1, &task_handle_button_a);
    xTaskCreate(button_b_task,     "ButtonB_Task",     256, NULL, 1, &task_handle_button_b);
    xTaskCreate(button_j_task,     "ButtonJ_Task",     256, NULL, 1, &task_handle_button_j);
    xTaskCreate(display_gate_task, "DisplayGate_Task", 256, NULL, 1, &task_handle_display_gate);
    xTaskCreate(mpu6500_task,      "MPU6500_Task",     512, NULL, 2, &task_handle_mpu6500);

    printf(":: Iniciando Scheduler ::\n");
    vTaskStartScheduler();

    printf(" :: Após iniciar o Scheduler, nunca chega esta linha ::\n");
    while (true) { }

    return 0;
}
