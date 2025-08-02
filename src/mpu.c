#include <stdio.h>
#include "mpu.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <math.h>
#include <stdint.h>   // ← Aqui garante o tipo uint8_t

// Escalas de conversão (g/LSB) para cada faixa
static const float accel_scales[] = {
    [ACCEL_RANGE_2G] = 16384.0f,
    [ACCEL_RANGE_4G] = 8192.0f,
    [ACCEL_RANGE_8G] = 4096.0f,
    [ACCEL_RANGE_16G] = 2048.0f
};

static float current_scale = 0;

bool mpu_i2c_init(i2c_inst_t *i2c, uint sda_pin, uint scl_pin, uint baudrate) {
    i2c_init(i2c, baudrate);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);
    return true;
}

static bool mpu_write_register(i2c_inst_t *i2c, uint8_t reg, uint8_t value) {
    uint8_t buf[] = {reg, value};
    int retorno = i2c_write_blocking(i2c, MPU6500_ADDR, buf, 2, false);
    printf("Retorno: %d \n", retorno);
    //return i2c_write_blocking(i2c, MPU6500_ADDR, buf, 2, false) == 2;
    return retorno;
}

static bool mpu_read_registers(i2c_inst_t *i2c, uint8_t reg, uint8_t *buf, uint8_t len) {
    if (i2c_write_blocking(i2c, MPU6500_ADDR, &reg, 1, true) != 1) {
        return false;
    }
    return i2c_read_blocking(i2c, MPU6500_ADDR, buf, len, false) == len;
}

bool mpu_init(i2c_inst_t *i2c) {
    // Resetar o dispositivo
    if (!mpu_write_register(i2c, MPU6500_PWR_MGMT_1, 0x80)) {
        printf("Falha 1 \n");
        return false;
    }
    sleep_ms(100);

    // Sair do modo sleep e usar clock interno
    if (!mpu_write_register(i2c, MPU6500_PWR_MGMT_1, 0x01)) {
        printf("Falha 2 \n");
        return false;
    }
    sleep_ms(10);


    /*

    // Verificar identificação do dispositivo
    uint8_t whoami;
    if (!mpu_read_registers(i2c, MPU6500_WHO_AM_I, &whoami, 1) || whoami != 0x70) {
        printf("Falha 3 \n");
        return false;
    }
    */

    return true;
}

bool mpu_set_accel_range(i2c_inst_t *i2c, accel_range_t range) {
    if (range > ACCEL_RANGE_16G) return false;

    if (!mpu_write_register(i2c, MPU6500_ACCEL_CONFIG, range << 3)) {
        return false;
    }

    current_scale = accel_scales[range];
    return true;
}

bool mpu_read_accel(i2c_inst_t *i2c, float accel[3]) {
    uint8_t buffer[6];

    if (!mpu_read_registers(i2c, MPU6500_ACCEL_XOUT_H, buffer, 6)) {
        return false;
    }

    // Converter bytes para valores inteiros (big-endian)
    int16_t raw_x = (buffer[0] << 8) | buffer[1];
    int16_t raw_y = (buffer[2] << 8) | buffer[3];
    int16_t raw_z = (buffer[4] << 8) | buffer[5];

    // Converter para g (considerando a escala atual)
    accel[0] = raw_x / current_scale;
    accel[1] = raw_y / current_scale;
    accel[2] = raw_z / current_scale;

    return true;
}
