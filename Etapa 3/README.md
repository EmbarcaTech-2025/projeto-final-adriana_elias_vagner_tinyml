<h1 align = "center"> Etapa 3 Prototipagem e Ajustes </h1>

Construir e testar o protótipo funcional, identificando ajustes necessários.

- **Sistema Embarcado:**
    - **Configurando FreeRTOS:** Implementar tarefas concorrentes;
    - **Implementando aquisição de dados:** Configurar MPU6500 via I2C, ajustando escala (±2g).
    - **Configurando display:** Inicializar SSD1306 para exibir a classe detectada (ex.: "Parado", "Subindo", etc.).
    - **Implementando MQTT:** Configurar cliente Paho MQTT, conectar ao broker (ex.: Mosquitto) e publicar mensagens com QoS 2.
    - **Otimizando energia:** Ativar modo sleep do RP2040 quando parado, reduzindo frequência de amostragem e desativando Wi-Fi.

# Modelo TinyML:

- Coletando dados: Simular movimentos em laboratório (parado, subindo/descendo com elevação manual, esquerda/direita com deslocamento linear, ziguezague com oscilações). Coletar 100 amostras por classe (1 min cada, 60 Hz).

- Pré-processando no Edge Impulse:
Filtro passa-baixa: Média móvel, frequência de corte 10 Hz.
Normalização: Escala [0,1] com base no intervalo de aceleração (±2g).
Janelamento: Janelas de 1s (60 amostras), sobreposição de 50%.

- Projetando modelo: Rede neural densa (32 neurônios x 2 camadas, ReLU, softmax).

- Treinando e avaliando: Usar validação cruzada, ajustando hiperparâmetros para acurácia > 80%.

- Convertendo modelo: Exportar para C++ (TensorFlow Lite Micro) e integrar ao firmware.

- Executando inferência: Testar latência no RP2040 (< 200 ms).

# Testes:

- Unitários: Aquisição de dados: Verificar frequência de 60 Hz, 
    - Pré-processamento: Validar filtro e janelamento.
    - Exibição: Confirmar atualização do SSD1306.
    - Transmissão: Testar mensagens MQTT com QoS 2.

- Testes de Integração:
    - Verificar interação entre tarefas FreeRTOS.
    - Testar inferência com modelo integrado.

- Campo: Simular movimentos reais (ex.: deslocar BitDogLab em carrinho para esquerda/direita, elevar para subindo/descendo).

- Métricas:
          - Acurácia: Comparar predições com rótulos reais.
          - Latência: Medir tempo de inferência.
          - Consumo: Monitorar corrente com multímetro.

- Documentando desafios: Ex.: ruído no MPU6500, falhas Wi-Fi, ajustes no modelo.


[![Apresentação](https://img.youtube.com/vi/r4KT6auRS9Y/hqdefault.jpg)](https://youtu.be/r4KT6auRS9Y "Etapa 3")

https://youtu.be/r4KT6auRS9Y?si=LlxuRj-Y9J4jt735


## Ajustes Etapa 4

- Testes de Estresse:
    - Operação por 48 horas em 0°C e 50°C.
    - Simular falhas Wi-Fi para verificar reconexão.
 
- Ajustes no código e melhorias na modulação

