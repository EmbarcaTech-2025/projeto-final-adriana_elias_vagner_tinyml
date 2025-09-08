/**
 * @file    button_a.c
 * @author  Adriana - Elias - Vagner
 * @brief   Task para o botão A
 * @version 0.1
 * @date    2025-09-08
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
//#include "queue.h"
#include "button_a.h"
#include "config.h"
#include "ctrl.h"
#include "led_rgb.h"
#include "util.h"

#define BUTTON BUTTON_A_GPIO


/**
 * @brief Task do botão A
 * Inicializa GPIO do botão
 * Monitora o estado do botão, que quando pressionado, envia para o display,
 * via pilha, as acelerações dos três eixos, por 5 segundos.
 */
void button_a_task(void *pvParameters){
    bool button_pressed = false;

    gpio_init(   BUTTON);
    gpio_pull_up(BUTTON);
    vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBONCE_MS)); // Delay para que o pull up faça efeito

    while(1){
        // Na BitDogLab, quando os botões (A e B) estão pressionados o I/O associado vai para nível baixo!
        if(!gpio_get(BUTTON)){  // Botao A pressionado!
            if(!button_pressed){
                button_pressed = true;
                util_gera_e_envia_msg(queue_handle, MSG_BUTTON_A_PRESSED);
                xSemaphoreTake(mutex_handler_led, portMAX_DELAY);
                util_gera_e_envia_msg(queue_handle, MSG_BUTTON_A_LED_ON);
                led_rgb_set(LED_COR_RED);
            }
        }else{  // Botao A NÃO pressionado!
            if(button_pressed){
                button_pressed = false;
                util_gera_e_envia_msg(queue_handle, MSG_BUTTON_A_RELEASE);
                led_rgb_set(LED_COR_BLACK);
                xSemaphoreGive(mutex_handler_led);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBONCE_MS));
    }
}
