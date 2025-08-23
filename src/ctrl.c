/**
 * @file    ctrl.c
 * @author  Ricardo e Vagner
 * @brief   Cria e disponibiliza variáveis de semáforo e pilha
 * @version 0.1
 * @date    2025-06-25
 */
#include "FreeRTOS.h"
#include "../include/config.h"
#include "../include/ctrl.h"

QueueHandle_t     queue_handle;
SemaphoreHandle_t mutex_handler_led;
SemaphoreHandle_t semaphored_handler_joystick;

/**
 * @brief Inicializa as variáveis de semáforo e pilha
 * 
 */
void ctrl_init(){
    mutex_handler_led           = xSemaphoreCreateMutex();
    semaphored_handler_joystick = xSemaphoreCreateBinary();
    queue_handle                = xQueueCreate(QUEUE_LENGHT, QUEUE_MSG_LENGHT);
}