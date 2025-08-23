/**
 * @file    display_gate.c
 * @author  Ricardo e Vagner
 * @brief   Task que mostra mensagens da file e requisições do semáforo no OLED. Simula um dispositivo lento
 * @version 0.1
 * @date    2025-06-25
 */
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "../include/config.h"
#include "../include/ctrl.h"
#include "../include/display_gate.h"
#include "oled/display.h"
#include "oled/i2c.h"

static char display_buffer[8][QUEUE_MSG_LENGHT] = {0};


/**
 * @brief Atualiza o display, simulando dispositivo lento
 * 
 * @param msg 
 */
static void add_msg(char * msg){
    // move as mensagens anterios para baixo
    for(int i=8;i>0;i--){
        for(int j=0;j<QUEUE_MSG_LENGHT;j++){
            display_buffer[i][j] = display_buffer[i-1][j];
        }
    }
    // adiciona a nova mensagem
    for(int i=0;i<QUEUE_MSG_LENGHT;i++){
        display_buffer[0][i] = msg[i];
    }
    // limpa o display
    display_fill(false, false);

    // preenche o display, menos a primeira linha
    for(int i=1;i<8;i++)
        display_msg(false, 0, i, display_buffer[i]);

    // marca a onde está escrevendo e depois escreve o valor para simular dispositivo lento
    bool reached_the_end = false;
    for(int i=0;i<16;i++){
        display_invert_char(true, i, 0);
        vTaskDelay(pdMS_TO_TICKS(50));
        display_invert_char(false, i, 0);
        if(!reached_the_end){
            char msg_c[2] = {0};
            msg_c[0] = display_buffer[0][i];
            if(msg_c[0]){
                display_msg(false, i, 0, msg_c);
            }else{
                reached_the_end = true;
            }
        }
        display_update();
    }
    vTaskDelay(pdMS_TO_TICKS(50));
}

/**
 * @brief Task que mostra mensagens da file e requisições do semáforo no OLED. Simula um dispositivo lento
 * 
 */
void display_gate_task(){
    char bufferMsg[QUEUE_MSG_LENGHT] = {0};

    display_init();
    display_msg(true, 5, 2, "RTOS 2");
    vTaskDelay(pdMS_TO_TICKS(1000));
    display_fill(false, false);
    display_msg(false, 0, 1, "    PRESSIONE");
    display_msg(false, 0, 3, "   ALGUM BOTAO");
    display_msg(false, 0, 5, "       OU");
    display_msg(true,  0, 7, "    JOYSTICK");

    while(1){
        if(xSemaphoreTake(semaphored_handler_joystick, 0) == pdTRUE){
            add_msg(MSG_BUTTON_SEMAPHORO);
        }else if (xQueueReceive(queue_handle, bufferMsg, 0) == pdTRUE) {
            add_msg(bufferMsg);
        }else{
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}