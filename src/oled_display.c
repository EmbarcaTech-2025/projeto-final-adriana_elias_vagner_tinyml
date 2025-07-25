#include "oled_display.h"
#include "hardware/i2c.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ssd1306_text.h" // Seu módulo de fontes
#include "hardware/gpio.h"


static void ssd1306_send_command(ssd1306_t *oled, uint8_t cmd) {
    oled->port_buffer[0] = 0x80;  // Controle: comando
    oled->port_buffer[1] = cmd;
    i2c_write_blocking(oled->i2c_port, oled->address, oled->port_buffer, 2, false);
}

static void ssd1306_send_command_list(ssd1306_t *oled, const uint8_t *cmds, size_t len) {
    for (size_t i = 0; i < len; i++) {
        ssd1306_send_command(oled, cmds[i]);
    }
}

bool oled_init(ssd1306_t *oled) {
    if (!oled) return false;

    oled->width = OLED_WIDTH;
    oled->height = OLED_HEIGHT;
    oled->pages = OLED_PAGES;
    oled->address = OLED_I2C_ADDR;
    oled->i2c_port = i2c1; // Fixo na BitDogLab
    oled->bufsize = oled->width * oled->pages + 1;

    oled->ram_buffer = calloc(oled->bufsize, sizeof(uint8_t));
    if (!oled->ram_buffer) return false;
    oled->ram_buffer[0] = 0x40; // Primeiro byte: controle para RAM

    // Setup dos pinos
    i2c_init(oled->i2c_port, OLED_I2C_FREQ);
    gpio_set_function(OLED_PIN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(OLED_PIN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_PIN_SDA);
    gpio_pull_up(OLED_PIN_SCL);

    // Sequência de comandos do SSD1306
    const uint8_t init_cmds[] = {
        0xAE, 0x20, 0x00, 0x40, 0xA1, 0xC8,
        0xDA, 0x12, 0x81, 0x7F, 0xA4, 0xA6,
        0xD5, 0x80, 0x8D, 0x14, 0xAF
    };

    ssd1306_send_command_list(oled, init_cmds, sizeof(init_cmds));
    return true;
}

void oled_clear(ssd1306_t *oled) {
    if (!oled || !oled->ram_buffer) return;
    memset(&oled->ram_buffer[1], 0x00, oled->bufsize - 1);
}

void oled_render(ssd1306_t *oled) {
    if (!oled || !oled->ram_buffer) return;

    ssd1306_send_command(oled, 0x21); // Set column address
    ssd1306_send_command(oled, 0);
    ssd1306_send_command(oled, oled->width - 1);

    ssd1306_send_command(oled, 0x22); // Set page address
    ssd1306_send_command(oled, 0);
    ssd1306_send_command(oled, oled->pages - 1);

    i2c_write_blocking(oled->i2c_port, oled->address, oled->ram_buffer, oled->bufsize, false);
}

