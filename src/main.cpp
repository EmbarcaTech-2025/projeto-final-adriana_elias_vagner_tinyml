/**
 * @file    main.cpp
 * @author  Adriana - Elias - Vagner
 * @brief   Ponto de entrada principal do sistema. Responsável por inicializar
 * o hardware básico, os módulos do FreeRTOS e criar todas as tarefas.
 * @version 0.1
 * @date    2025-14-09
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "FreeRTOS.h"
#include "task.h"

#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <pico/stdio_usb.h>


#include "semphr.h"
#include "queue.h"
#include "ml.h"

// Trate todas as funções declaradas neste arquivo como funções C puras, ...
// ... sem modificar seus nomes ("name mangling")
// Inclusão dos cabeçalhos de todas as tarefas e módulos do projeto
extern "C" {
#include "button_a.h"
#include "button_b.h"
#include "button_j.h"
#include "config.h"
#include "ctrl.h"
#include "display_gate.h"
#include "led_rgb.h"
#include "mpu6500.h"
#include "mqtt_task.h"    // Tarefa de Rede
#include "include/ml.h"   // Tarefa de Machine Learning
#include "wifi_config.h"  // Configurações Wi-Fi (SSID, PASS)
}
//#include "edge-impulse-sdk/classifier/ei_run_classifier.h"


void wifi_init_task(void *params);

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
    TaskHandle_t task_handle_mqtt;
    TaskHandle_t task_handle_wifi;

    // Inicializa a E/S padrão (para que o printf funcione via USB)
    stdio_init_all();
    // Adiciona um delay generoso para dar tempo ao monitor serial de se conectar.
    // Essencial para não perder as primeiras mensagens de log.
    sleep_ms(5000);
    printf("\n========================================\n");
    printf("Embarcatech Fase-2 :: Projeto Final\n");
    printf("========================================\n");
    printf("MAIN: A iniciar o sistema...\n");

    printf("MAIN: A inicializar periféricos e controle RTOS...\n");
    stdio_usb_init();
    ctrl_init();

    // Cria uma tarefa temporária para inicializar o Wi-Fi
    xTaskCreate(wifi_init_task,     "WiFi",          2048, NULL, 1, &task_handle_wifi);

    led_rgb_init();
    mpu6500_init(); // Inicializa o acelerômetro

    printf(":: Criando as tarefas ::\n");
    xTaskCreate(button_a_task,     "ButtonA_Task",     256, NULL, 1, &task_handle_button_a);
    xTaskCreate(button_b_task,     "ButtonB_Task",     256, NULL, 1, &task_handle_button_b);
    xTaskCreate(button_j_task,     "ButtonJ_Task",     256, NULL, 1, &task_handle_button_j);
    xTaskCreate(display_gate_task, "DisplayGate_Task", 256, NULL, 1, &task_handle_display_gate);// Tarefa display gate
    xTaskCreate(mpu6500_task,      "MPU6500_Task",     512, NULL, 1, &task_handle_mpu6500);     // Tarefa acelerômetro
    xTaskCreate(ml_task,           "ML_Task",          512, NULL, 1, &task_handle_ml);          // Task de Machine Learning
    xTaskCreate(mqtt_task,         "MQTT_Task",        512, NULL, 1, &task_handle_mqtt);        // Tarefa de rede

    printf(":: Iniciando Scheduler ::\n");
    vTaskStartScheduler();

    printf(" :: Após iniciar o Scheduler, nunca chega esta linha ::\n");

    while (true) {
        printf("ERRO CRÍTICO: Falha ao iniciar o escalonador do FreeRTOS!\n");
        sleep_ms(5000);
    }
    return 0;
}


void wifi_init_task(void *params) {
    printf("WIFI_INIT: Iniciando hardware Wi-Fi...\n");

    if (cyw43_arch_init()) {
        printf("WIFI_INIT ERRO: Falha ao inicializar hardware Wi-Fi.\n");
        // LED de erro ou outra indicação
        while(1) {
            sleep_ms(1000);
        }
    }

    printf("WIFI_INIT: Habilitando modo station...\n");
    cyw43_arch_enable_sta_mode();

    printf("WIFI_INIT: Conectando à rede '%s'...\n", SSID);
    if (cyw43_arch_wifi_connect_blocking(SSID, PASS, CYW43_AUTH_WPA2_AES_PSK)) {
        printf("WIFI_INIT ERRO: Falha ao conectar à rede Wi-Fi.\n");
        // Tratar erro de conexão
        while(1) {
            sleep_ms(1000);
        }
    }

    printf("WIFI_INIT: Wi-Fi conectado com sucesso!\n");
    vTaskDelete(NULL); // Encerra a tarefa após conectar
}
