#include "MQTTClient.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include <stdio.h>

// Inclusão dos cabeçalhos da biblioteca Paho MQTT e das nossas implementações de rede.
#include "paho_network.h"
#include "paho_timer.h"

// ========================================================================
// CONFIGURAÇÕES DE REDE e MQTT
// ========================================================================

#define WIFI_SSID           "ALEX_2G"
#define WIFI_PASSWORD       "12345678"

// Servidor MQTT (SUBSTITUIR PELO SEU BROKER)
#define MQTT_BROKER_HOST            "bbbf88f16bbe4aa6ba8a25a2adde1d65.s1.eu.hivemq.cloud"
#define MQTT_BROKER_PORT            1883
#define MQTT_BROKER_PORT_SSL        8883        // Porta SSL/TLS

// Configurações de autenticação MQTT
#define MQTT_USE_AUTH               1       // true para habilitar autenticação
#define MQTT_USERNAME               "IotmyDevices"
#define MQTT_PASSWORD               "AtLrfD8GA4@VEK#"

// Configurações de conexão MQTT
#define MQTT_CLIENT_ID_PREFIX       "tinyml_gate_"
#define MQTT_CLIENT_ID              "tinyml_gate_pico_001"
#define MQTT_KEEP_ALIVE_SEC         60          // Keep alive: 60 segundos
#define MQTT_CONNECT_TIMEOUT_MS     10000       // Timeout conexão: 10s
#define MQTT_QOS_LEVEL              0           // QoS 0: At most once delivery

// Tópicos MQTT
#define MQTT_TOPIC_ROOT             "tinyml/iot/myDevices"
#define MQTT_TOPIC_DATA             MQTT_TOPIC_ROOT "/data"
#define MQTT_TOPIC_ALERTS           MQTT_TOPIC_ROOT "/alerts"
#define MQTT_TOPIC_STATUS           MQTT_TOPIC_ROOT "/status"
#define MQTT_TOPIC_CONFIG           MQTT_TOPIC_ROOT "/config"
#define MQTT_TOPIC_COMMANDS         MQTT_TOPIC_ROOT "/commands"

// Para teste
#define MQTT_SUB_TOPIC              MQTT_TOPIC_COMMANDS
#define MQTT_PUB_TOPIC              MQTT_TOPIC_STATUS

// Configurações de buffer MQTT
#define MQTT_BUFFER_SIZE            1024        // Buffer para mensagens MQTT
#define MQTT_MAX_MESSAGE_SIZE       512         // Tamanho máximo de mensagem

// Buffer de rede e MQTT
static unsigned char sendbuf[512];
static unsigned char readbuf[512];

// Função para verificar status do WiFi
static bool check_wifi_status(void) {
    int status = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
    printf("WiFi status: %d\n", status);
    return (status == CYW43_LINK_UP);
}

// Função callback para mensagens recebidas
void messageArrived(MessageData* data) {
    printf("=== Mensagem MQTT Recebida ===\n");
    printf("Tópico: %.*s\n", data->topicName->lenstring.len, data->topicName->lenstring.data);
    printf("Payload: %.*s\n", (int)data->message->payloadlen, (char*)data->message->payload);
    printf("=============================\n");
}

// Task MQTT
void mqtt_task(void *pvParameters) {
    int rc = 0;
    Network network;
    MQTTClient client;
    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

    printf("\n=== MQTT Task Iniciada ===\n");
    printf("Stack livre: %d bytes\n", uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t));
    printf("Heap livre: %d bytes\n", xPortGetFreeHeapSize());

    // Aguarda WiFi estar conectado
    printf("Aguardando WiFi estar conectado...\n");
    int wifi_retry = 0;
    while (!check_wifi_status() && wifi_retry < 30) {
        printf("WiFi não conectado, tentativa %d/30\n", wifi_retry + 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        wifi_retry++;
    }

    if (wifi_retry >= 30) {
        printf("ERRO: WiFi não conectou após 30 tentativas\n");
        vTaskDelete(NULL);
        return;
    }

    printf("WiFi conectado! Prosseguindo com MQTT...\n");

    // Inicializa rede usando nossa implementação
    printf("Inicializando estrutura de rede...\n");
    NetworkInit(&network);
    
    printf("Tentando conectar ao broker %s:%d\n", MQTT_BROKER_HOST, MQTT_BROKER_PORT);
    
    if ((rc = NetworkConnect(&network, MQTT_BROKER_HOST, MQTT_BROKER_PORT)) != 0) {
        printf("ERRO: Falha ao conectar no broker %s:%d (código: %d)\n", MQTT_BROKER_HOST, MQTT_BROKER_PORT, rc);
        vTaskDelete(NULL);
        return;
    }
    printf("✓ Conectado ao broker TCP %s:%d\n", MQTT_BROKER_HOST, MQTT_BROKER_PORT);

    // Inicializa cliente MQTT
    printf("Inicializando cliente MQTT...\n");
    MQTTClientInit(&client, &network, MQTT_CONNECT_TIMEOUT_MS, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Configura dados de conexão
    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = MQTT_CLIENT_ID;
    connectData.keepAliveInterval = MQTT_KEEP_ALIVE_SEC;
    connectData.cleansession = 1;

    // Configurar autenticação se habilitada
    #if MQTT_USE_AUTH
    connectData.username.cstring = MQTT_USERNAME;
    connectData.password.cstring = MQTT_PASSWORD;
    printf("Usando autenticação: usuário=%s\n", MQTT_USERNAME);
    #endif

    printf("Conectando cliente MQTT (timeout: %dms)...\n", MQTT_CONNECT_TIMEOUT_MS);
    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("ERRO: Falha ao conectar MQTT, código %d\n", rc);
        NetworkDisconnect(&network);
        vTaskDelete(NULL);
        return;
    }
    printf("✓ Cliente MQTT conectado com sucesso!\n");

    // Subscribe em um tópico
    printf("Subscrevendo em %s...\n", MQTT_SUB_TOPIC);
    if ((rc = MQTTSubscribe(&client, MQTT_SUB_TOPIC, QOS0, messageArrived)) != 0) {
        printf("ERRO: Falha ao subscrever, código %d\n", rc);
    } else {
        printf("✓ Subscrito em %s com sucesso\n", MQTT_SUB_TOPIC);
    }

    printf("=== MQTT Loop Principal Iniciado ===\n");

    // Loop principal
    int count = 0;
    while (1) {
        // Verifica status do WiFi periodicamente
        if (count % 60 == 0) {  // A cada 60 segundos
            if (!check_wifi_status()) {
                printf("ERRO: WiFi desconectado!\n");
                break;
            }
        }

        // Mantém a conexão viva e processa mensagens recebidas
        if ((rc = MQTTYield(&client, 1000)) != 0) {
            printf("ERRO no MQTTYield: %d\n", rc);
            break;
        }

        // Publica mensagem de teste a cada 10 segundos
        if (++count >= 10) {
            MQTTMessage message;
            char payload[128];
            
            // Informações do sistema
            uint32_t tick = xTaskGetTickCount();
            int heap_free = xPortGetFreeHeapSize();
            int stack_free = uxTaskGetStackHighWaterMark(NULL) * sizeof(StackType_t);
            
            snprintf(payload, sizeof(payload), 
                    "{\"device\":\"pico_w\",\"status\":\"online\",\"tick\":%lu,\"heap\":%d,\"stack\":%d}", 
                    tick, heap_free, stack_free);

            message.qos = QOS0;
            message.retained = 0;
            message.payload = payload;
            message.payloadlen = strlen(payload);

            printf("Publicando em %s:\n%s\n", MQTT_PUB_TOPIC, payload);
            if ((rc = MQTTPublish(&client, MQTT_PUB_TOPIC, &message)) != 0) {
                printf("ERRO ao publicar, código %d\n", rc);
            } else {
                printf("✓ Mensagem publicada com sucesso\n");
            }
            count = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Cleanup em caso de erro
    printf("=== MQTT Task Finalizando ===\n");
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    printf("MQTT desconectado. Task finalizada.\n");
    vTaskDelete(NULL);
}