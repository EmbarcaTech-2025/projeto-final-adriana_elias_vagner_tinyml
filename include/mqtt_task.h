/**
 * @file    mqtt_task.h
 * @author  Adriana - Elias - Vagner
 * @brief   Header para a task MQTT
 * @version 0.1
 * @date
 */
#ifndef MQTT_TASK_H
#define MQTT_TASK_H

/**
 * @brief Task MQTT principal
 * Gerencia a conexão e comunicação MQTT
 * @param pvParameters Parâmetros da task (não usado)
 */
void mqtt_task(void *pvParameters);

#endif // MQTT_TASK_H