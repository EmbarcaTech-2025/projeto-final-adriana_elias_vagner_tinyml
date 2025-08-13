/**
 * @file    button_a.c
 * @author  Ricardo e Vagner
 * @brief   Task para o botão A
 * @version 0.1
 * @date    2025-06-25
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "../include/button_a.h"
#include "../include/config.h"
#include "../include/ctrl.h"
#include "../include/led_rgb.h"
#include "../include/util.h"

#define BUTTON BUTTON_A_GPIO


/**
 * @brief Task do botão A
 * Inicializa GPIO do botão
 * Monitora transições de estado do,botão :
 * - quando pressionado:
 *   - envia mensagem que foi pressionado para o display via pilha;
 *   - aguarda o LED estar disponível(MUTEX) para acessá-lo e manda uma nova mensagem para o display via pilha;
 * - quando solto:
 *   - libera o acesso ao LED(MUTEX);
 *   - Manda uma mensagem para o display via pilha
 */
void button_a_task(){
    bool button_pressed = false;

    gpio_init(   BUTTON);
    gpio_pull_up(BUTTON);
    vTaskDelay(pdMS_TO_TICKS(BUTTON_DEBONCE_MS)); // Delay para que o pull up faça efeito

    while(1){
        if(!gpio_get(BUTTON)){
            if(!button_pressed){
                button_pressed = true;
                util_gera_e_envia_msg(queue_handle, MSG_BUTTON_A_PRESSED);
                xSemaphoreTake(mutex_handler_led, portMAX_DELAY);
                util_gera_e_envia_msg(queue_handle, MSG_BUTTON_A_LED_ON);
                led_rgb_set(LED_COR_RED);
            }
        }else{
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