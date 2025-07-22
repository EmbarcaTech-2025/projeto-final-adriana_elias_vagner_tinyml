#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

// Módulos do projeto
#include "i2c_config.h"
#include "mpu.h"
#include "oled_display.h"
#include "oled_context.h"
#include "display_oled.h"
#include "buffer_sensor.h"
#include "leitura_sensor.h"
#include "tarefa_botao_a.h"
#include "tarefa_calibracao.h"
#include "handles_tarefas.h"

// Handles globais
TaskHandle_t handle_display       = NULL;
TaskHandle_t handle_botao_a       = NULL;
TaskHandle_t handle_leitura_mpu   = NULL;

void main() {
    stdio_init_all();
    // Aguarda conexão USB se necessário:
    // while (!stdio_usb_connected()) { sleep_ms(10); }

    printf("=== INICIANDO SISTEMA MPU + OLED ===\n");

    // Configura os barramentos I2C
    i2c_configurar(i2c0, 0, 1);     // SDA0 = GPIO 0, SCL0 = GPIO 1
    i2c_configurar(i2c1, 14, 15);   // SDA1 = GPIO 14, SCL1 = GPIO 15
    printf("Barramentos I2C inicializados\n");

    // Inicializa buffer circular de leituras
    buffer_sensor_inicializar();

    // Inicializa sensor MPU
    if (!mpu_inicializar(i2c0)) {
        printf("Sensor MPU não detectado.\n");
        while (true);
    }

    // Inicializa display OLED
    if (!oled_init(&oled)) {
        printf("Falha ao inicializar OLED!\n");
        while (true);
    }

    // Cria mutex para uso compartilhado do OLED
    mutex_oled = xSemaphoreCreateMutex();
    if (mutex_oled == NULL) {
        printf("Erro ao criar mutex do OLED\n");
        while (true);
    }

    // Tarefa de calibração (núcleo 1)
    xTaskCreateAffinitySet(
        tarefa_calibracao, "Calibracao", 2048, i2c0,
        1, (1 << 1), NULL
    );

    // Tarefa de leitura da MPU (núcleo 0)
    xTaskCreateAffinitySet(
        tarefa_leitura_mpu, "LeituraMPU", 1024, NULL,
        1, (1 << 0), &handle_leitura_mpu
    );

    // Tarefa de exibição centralizada (núcleo 1)
    xTaskCreateAffinitySet(
        tarefa_display_oled, "DisplayOLED", 1024, NULL,
        1, (1 << 1), &handle_display
    );

    // Tarefa do botão A (núcleo 0)
    xTaskCreateAffinitySet(
        tarefa_botao_a, "BotaoA", 1024, NULL,
        1, (1 << 0), &handle_botao_a
    );

    // Suspende tarefas de exibição e botão até fim da calibração
    vTaskSuspend(handle_display);
    vTaskSuspend(handle_botao_a);

    // Inicia o escalonador do FreeRTOS
    vTaskStartScheduler();

    while (true); // Nunca deve chegar aqui
}
