/**
 * @file    main.cpp
 * @author  Adriana - Elias - Vagner
 * @brief
 * @version 0.1
 * @date    2025-09-08
 */
#include <stdio.h>
#include <hardware/gpio.h>
#include <hardware/uart.h>
#include <pico/stdio_usb.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

extern "C" {
#include "button_a.h"
#include "button_b.h"
#include "button_j.h"
#include "config.h"
#include "ctrl.h"
#include "display_gate.h"
#include "led_rgb.h"
#include "mpu6500.h"
}
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"

//const uint LED_PIN = 25;

// Copy raw features here (e.g. from the 'Model testing' page)
static const float features[] = {0.2686, 0.6005, 1.4064, 0.2449, 0.6005, 1.3827, 0.3081, 0.5136, 1.3748, 0.3002, 0.6321, 1.3906, 0.2449, 0.5768, 1.3985, 0.3239, 0.6400, 1.4222, 0.2686, 0.5136, 1.4617, 0.3002, 0.5847, 1.4459, 0.2765, 0.5768, 1.3431, 0.2449, 0.6084, 1.3510, 0.3160, 0.6321, 1.4064, 0.3239, 0.6479, 1.3827, 0.2607, 0.5926, 1.4301, 0.2607, 0.5136, 1.3985, 0.2370, 0.5689, 1.4064, 0.3239, 0.6795, 1.4696, 0.3002, 0.5373, 1.3748, 0.2923, 0.6084, 1.3036, 0.3160, 0.6242, 1.4301, 0.3160, 0.6163, 1.4538, 0.2370, 0.5689, 1.4222, 0.2449, 0.6400, 1.4064, 0.2370, 0.5926, 1.3827, 0.3634, 0.5847, 1.3194, 0.2607, 0.5610, 1.3748, 0.2212, 0.5294, 1.3194, 0.2923, 0.5768, 1.3906, 0.3318, 0.5610, 1.3510, 0.2765, 0.5768, 1.3748, 0.2607, 0.6558, 1.4064, 0.2291, 0.5847, 1.4617, 0.3318, 0.6005, 1.4064, 0.2765, 0.5689, 1.3510, 0.2449, 0.5926, 1.4064, 0.2291, 0.5926, 1.4459, 0.3081, 0.6479, 1.4380, 0.2686, 0.6005, 1.4143, 0.2686, 0.6163, 1.4222, 0.2844, 0.6479, 1.4459, 0.2607, 0.6795, 1.3748, 0.3476, 0.6321, 1.3668, 0.2844, 0.6242, 1.3748};


int raw_feature_get_data(size_t offset, size_t length, float *out_ptr) {
  memcpy(out_ptr, features + offset, length * sizeof(float));
  return 0;
}

void print_inference_result(ei_impulse_result_t result) {

    // Print how long it took to perform inference
    ei_printf("Timing: DSP %d ms, inference %d ms, anomaly %d ms\r\n",
            result.timing.dsp,
            result.timing.classification,
            result.timing.anomaly);

    // Print the prediction results (object detection)
#if EI_CLASSIFIER_OBJECT_DETECTION == 1
    ei_printf("Object detection bounding boxes:\r\n");
    for (uint32_t i = 0; i < result.bounding_boxes_count; i++) {
        ei_impulse_result_bounding_box_t bb = result.bounding_boxes[i];
        if (bb.value == 0) {
            continue;
        }
        ei_printf("  %s (%f) [ x: %u, y: %u, width: %u, height: %u ]\r\n",
                bb.label,
                bb.value,
                bb.x,
                bb.y,
                bb.width,
                bb.height);
    }

    // Print the prediction results (classification)
#else
    ei_printf("Predictions:\r\n");
    for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        ei_printf("  %s: ", ei_classifier_inferencing_categories[i]);
        ei_printf("%.5f\r\n", result.classification[i].value);
    }
#endif

    // Print anomaly result (if it exists)
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("Anomaly prediction: %.3f\r\n", result.anomaly);
#endif

}

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

    stdio_usb_init();
    sleep_ms(3000);

    printf("Embarcatech Fase-2 :: Projeto Final\n");

    ctrl_init();
    led_rgb_init();
    mpu6500_init(); // Inicializa o acelerômetro

    printf(":: Criando as tarefas ::\n");
    xTaskCreate(button_a_task,     "ButtonA_Task",     256, NULL, 1, &task_handle_button_a);
    xTaskCreate(button_b_task,     "ButtonB_Task",     256, NULL, 1, &task_handle_button_b);
    xTaskCreate(button_j_task,     "ButtonJ_Task",     256, NULL, 1, &task_handle_button_j);
    xTaskCreate(display_gate_task, "DisplayGate_Task", 256, NULL, 1, &task_handle_display_gate);
    xTaskCreate(mpu6500_task,      "MPU6500_Task",     512, NULL, 2, &task_handle_mpu6500);

    printf(":: Iniciando Scheduler ::\n");
    vTaskStartScheduler();

    printf(" :: Após iniciar o Scheduler, nunca chega esta linha ::\n");

    //gpio_init(LED_PIN);
    //gpio_set_dir(LED_PIN, GPIO_OUT);

    ei_impulse_result_t result = {nullptr};

    ei_printf("Edge Impulse standalone inferencing (Raspberry Pi Pico)\n");

    if (sizeof(features) / sizeof(float) != EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
        ei_printf("The size of your 'features' array is not correct. Expected %d items, but had %u\n",
        EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, sizeof(features) / sizeof(float));
        return 1;
    }

    while (true) {
        // blink LED
        //gpio_put(LED_PIN, !gpio_get(LED_PIN));

        // the features are stored into flash, and we don't want to load everything into RAM
        signal_t features_signal;
        features_signal.total_length = sizeof(features) / sizeof(features[0]);
        features_signal.get_data = &raw_feature_get_data;

        // invoke the impulse
        EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false /* debug */);
        if (res != EI_IMPULSE_OK) {
            ei_printf("ERR: Failed to run classifier (%d)\n", res);
            return res;
        }

        print_inference_result(result);
        ei_sleep(2000);
    }
}
