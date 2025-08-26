/**
 * @file    display_gate.c
 * @author
 * @brief   Task que mostra mensagens da file e requisições do semáforo no OLED.
 * Simula um dispositivo lento
 * @version 0.1
 * @date
 */
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include <stdio.h> // Para snprintf
#include "../include/config.h"
#include "../include/ctrl.h"
#include "../include/display_gate.h"
#include "../include/mpu6500.h"
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
    for(int i=0;i<QUEUE_MSG_LENGHT-1;i++){
        display_buffer[0][i] = msg[i];
    }
    // limpa o display
    display_fill(false, false);

    // preenche o display, menos a primeira linha
    for(int i=1;i<8;i++)
        display_msg(false, 0, i, display_buffer[i]);

    // Marca a onde está escrevendo e depois escreve o valor para simular dispositivo lento
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
 * @brief Task que mostra mensagens da file e requisições do semáforo no OLED.
 * Simula um dispositivo lento
 */
void display_gate_task(){
    char bufferMsg[QUEUE_MSG_LENGHT] = {0};
    // Estrutura para armazenar os dados do MPU6500
    mpu6500_data_t mpu_data;
    // Obtém o handle da fila do MPU6500.
    // É necessário um cast pois mpu6500_get_queue retorna void*.
    QueueHandle_t mpu_queue_handle = (QueueHandle_t)mpu6500_get_queue();

    display_init();
    display_msg(true, 1, 1, "::  TinyML  ::");
    display_msg(true, 1, 3, "   Movement   ");
    display_msg(true, 1, 5, "Classification");
    vTaskDelay(pdMS_TO_TICKS(1000));
    display_fill(false, false);
    // display_msg(false, 0, 1, "    PRESSIONE");
    // display_msg(false, 0, 3, "   ALGUM BOTAO");
    // display_msg(false, 0, 5, "       OU");
    // display_msg(true,  0, 7, "    JOYSTICK");

    while(1){
        if(xSemaphoreTake(semaphored_handler_joystick, 0) == pdTRUE){
            add_msg(MSG_BUTTON_SEMAPHORO);
        }else if (xQueueReceive(queue_handle, bufferMsg, 0) == pdTRUE) {
            add_msg(bufferMsg);
        }else if (xQueueReceive(mpu_queue_handle, &mpu_data, 0) == pdTRUE) { // Tenta receber dados do MPU6500
            char accel_msg[QUEUE_MSG_LENGHT];
            // Formata os dados de aceleração em uma string
            // snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ax:%.2f Ay:%.2f Az:%.2f",
            //          mpu_data.accel_x_g, mpu_data.accel_y_g, mpu_data.accel_z_g);
            // add_msg(accel_msg); // Exibe a mensagem no display
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ax:%.4f",mpu_data.accel_x_g);
            add_msg(accel_msg); // Exibe a mensagem no display
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Ay:%.4f",mpu_data.accel_y_g);
            add_msg(accel_msg); // Exibe a mensagem no display
            snprintf(accel_msg, QUEUE_MSG_LENGHT, "Az:%.4f",mpu_data.accel_z_g);
            add_msg(accel_msg); // Exibe a mensagem no display
        }else{
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}
