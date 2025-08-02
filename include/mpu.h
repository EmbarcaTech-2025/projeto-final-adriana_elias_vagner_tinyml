#ifndef MPU_H
#define MPU_H

#include "hardware/i2c.h"
#include <stdbool.h>
#include <stdint.h>


// Endereço I2C
#define MPU6500_ADDR 0x68

// Registradores
#define MPU6500_WHO_AM_I      0x75
#define MPU6500_PWR_MGMT_1    0x6B
#define MPU6500_ACCEL_CONFIG  0x1C
#define MPU6500_ACCEL_XOUT_H  0x3B

// Faixas de medida do acelerômetro
typedef enum {
    ACCEL_RANGE_2G = 0,
    ACCEL_RANGE_4G,
    ACCEL_RANGE_8G,
    ACCEL_RANGE_16G
} accel_range_t;

// Protótipos das funções
bool mpu_i2c_init(i2c_inst_t *i2c, uint sda_pin, uint scl_pin, uint baudrate);
bool mpu_init(i2c_inst_t *i2c);
bool mpu_set_accel_range(i2c_inst_t *i2c, accel_range_t range);
bool mpu_read_accel(i2c_inst_t *i2c, float accel[3]);

#endif // MPU_H
