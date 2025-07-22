#include "tarefa_botao_a.h"
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "tela_display.h"

#define GPIO_BOTAO_A 5

void tarefa_botao_a(void *params) {
    gpio_init(GPIO_BOTAO_A);
    gpio_set_dir(GPIO_BOTAO_A, GPIO_IN);
    gpio_pull_up(GPIO_BOTAO_A);

    bool pressionado = false;

    while (true) {
        bool estado = (gpio_get(GPIO_BOTAO_A) == 0);
        vTaskDelay(pdMS_TO_TICKS(10));

        if (estado && !pressionado) {
            pressionado = true;

            // Avança para próxima tela
            tela_atual = (tela_atual + 1) % TOTAL_TELAS;
            printf("[Botao A] Nova tela: %d\n", tela_atual);
        }

        if (!estado && pressionado) {
            pressionado = false;
        }

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
