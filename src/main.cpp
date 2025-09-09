/**
 * @file    main.cpp
 * @author  Adriana - Elias - Vagner
 * @brief
 * @version 0.1
 * @date    2025-09-08
 */
#include <stdio.h>
#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <pico/stdio_usb.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "ml.h"

// Trate todas as funções declaradas neste arquivo como funções C puras, sem modificar seus nomes ("name mangling")
extern "C" {
#include "button_a.h"
#include "button_b.h"
#include "button_j.h"
#include "config.h"
#include "ctrl.h"
#include "display_gate.h"
#include "led_rgb.h"
#include "mpu6500.h"
}
//#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

/**
 * @brief Inicializa programa
 * - inicializa elementos de hardware que não fazem parte de tasks
 * - cria e inicializa semáforos
 * - cria tasks
 * - inicializa scheduler
 *
 * @return int
 */
int main() {
    TaskHandle_t task_handle_button_a;
    TaskHandle_t task_handle_button_b;
    TaskHandle_t task_handle_button_j;
    TaskHandle_t task_handle_display_gate;
    TaskHandle_t task_handle_mpu6500;
    TaskHandle_t task_handle_ml;

    stdio_usb_init();
    sleep_ms(3000);   // Para abrir o terminal

    printf("Embarcatech Fase-2 :: Projeto Final\n");

    ctrl_init();
    led_rgb_init();
    mpu6500_init(); // Inicializa o acelerômetro

    printf(":: Criando as tarefas ::\n");
    xTaskCreate(button_a_task,     "ButtonA_Task",     256, NULL, 1, &task_handle_button_a);
    xTaskCreate(button_b_task,     "ButtonB_Task",     256, NULL, 1, &task_handle_button_b);
    xTaskCreate(button_j_task,     "ButtonJ_Task",     256, NULL, 1, &task_handle_button_j);
    xTaskCreate(display_gate_task, "DisplayGate_Task", 256, NULL, 1, &task_handle_display_gate);
    xTaskCreate(mpu6500_task,      "MPU6500_Task",     512, NULL, 1, &task_handle_mpu6500);
    xTaskCreate(ml_task,           "ML_Task",          512, NULL, 1, &task_handle_ml);


    printf(":: Iniciando Scheduler ::\n");
    vTaskStartScheduler();

    printf(" :: Após iniciar o Scheduler, nunca chega esta linha ::\n");

    while (true) { }
    return 0;
}
