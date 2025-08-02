#include <pico/time.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu.h"
#include "pico/binary_info.h"


#define I2C_PORT i2c0
#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 1

int main() {
    stdio_init_all();
    sleep_ms(10000);

    // Inicializa I2C
    if (!mpu_i2c_init(I2C_PORT, I2C_SDA_PIN, I2C_SCL_PIN, 100000)) {
        printf("Falha na inicializacao do I2C\n");
        return -1;
    }

    // Inicializa MPU6500
    if (!mpu_init(I2C_PORT)) {
        printf("Falha na inicializacao do MPU6500\n");
        return -1;
    }

    // Configura faixa de medida do acelerômetro (±8g)
    mpu_set_accel_range(I2C_PORT, ACCEL_RANGE_8G);

    // Loop principal
    while (true) {
        float accel[3];
        if (mpu_read_accel(I2C_PORT, accel)) {
            //printf("Aceleracao: X=%.2fg, Y=%.2fg, Z=%.2fg\n",
            //      accel[0], accel[1], accel[2]);
            printf("%.2f \t", accel[0]);
            printf("%.2f \t", accel[1]);
            printf("%.2f \t", accel[2]);
            printf("\n");
        } else {
            printf("Erro na leitura do acelerometro\n");
        }

        sleep_ms(100); // Taxa de amostragem de 10Hz
    }

    return 0;
}
