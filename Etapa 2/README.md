<h1 align = "center"> Etapa 2: Arquitetura e Modelagem </h1>

**Objetivo:** Definir a arquitetura do sistema e modelar o software e o modelo TinyML.

### Diagrama de hardware
A figura 3 apresenta o diagrama de hardware proposto.
![diagrama](/assets/esquematico.png)
<h4 align = "right"> Figura 3 - Fonte: produzido pelos autores </h4>

Ambos os componentes utilizados (Acelerômetro MPU6500 e Display SSD1306) possuem interface serial de comunicação do tipo I2C. Na BitDogLab, o display já está conectado na interface I2C1 (GPIO 14 e 15); conectaremos o acelerômetro na interface I2C0 (GP0 e GP1), sendo o RP2040, em ambos o caso, o mestre da rede e os componentes os escravos.

Endereços de rede:
|Componente|Endereço|
|---|---|
|MPU6050|0x68|
|SSD1306|0x3C|

O módulo wi-fi (CYW43439) já é integrado a placa Pi Pico W, disponibilizada na BitDogLab.

### Blocos funcionais do software
A figura 4 apresenta o diagrama de blocos funcionais do software proposto.
![diagrama](/assets/etapa_1_diagrama.png)
<h4 align = "right"> Figura 4 - Produzido pelos autores </h4>

Conforme apresentado na figura 4, a proposta envolve a utilização de um sistema operacional de tempo real (FreeRTOS) para o controle do sistema. Serão desenvolvidas as seguintes tarefas:
|Tarefa|Função|
|---|---|
|Wi-fi_task|Realiza autenticação no ponto de acesso wi-fi|
|mqtt_task|Envia os dados para o broker mqtt na nuvem e para a a tarefa display_gate_task|
|display_gate_task|Envia os dados para o display|
|mpu6500_task|Lê os dados do acelerômetro e os envia para a tarefa ML_task|
|ML_task| Recebe os dados do acelerômetro e realiza a classsificação do movimento (inferência) e envia o resultado para o wi-fi e display|

Temos ainda o **Semaphoro**, que atua como mecanismo de sinalização entre as tarefas: Wi-fi_task e mqtt_task, de forma que a tarefa mqtt_task aguarda a autenticação da Wi-fi_task para iniciar; e a fila **Queue** que controla o acesso ao display.

### Fluxograma do software

<img width="1101" height="1441" alt="fluxograma2222" src="https://github.com/user-attachments/assets/42490ab9-bdc4-4ea6-9dc2-477b6de5850a" />


- Inicialização: Configurar FreeRTOS, inicializar periféricos (MPU6500, SSD1306, Wi-Fi).
- Loop Principal:
1. Autenticação nno ponto de acesso;
2. Ler dados do MPU6500;
3. Pré-processar os dados (filtro passa-baixa, normalização, janelamento, outros);
4. Executar inferência com o modelo de ML (TinyML);
5. Exibir classe no SSD1306;
6. Transmitir via MQTT para o broker MQTT;
7. Verificar estado parado para modo de baixo consumo.

- Configurando ambientes:
1. Instalar Pico-SDK e configurar FreeRTOS no RP2040;
2. Configurar Edge Impulse para coleta de dados e treinamento do modelo.

- Definindo arquitetura do modelo TinyML:
1. Pré-processamento;
2. Modelo: Rede neural densa;
3. Saída: Probabilidades para as classes: parado, subindo/descendo, esquerda/direita, ziguezague.
___________________________________________________________________________________________

Explicação da Lógica e Estrutura do Sistema

A arquitetura de software proposta foi projetada para ser modular, escalável e robusta, atendendo a todos os requisitos funcionais e não funcionais do projeto. A escolha central da arquitetura é o uso de um Sistema Operacional de Tempo Real (RTOS), especificamente o FreeRTOS, que permite gerenciar as múltiplas atividades do sistema de forma concorrente e organizada.

Arquitetura Baseada em Tarefas Concorrentes

O sistema é decomposto em cinco tarefas principais, cada uma com uma responsabilidade única. Essa abordagem, fundamentada no FreeRTOS, permite que operações lentas (como comunicação Wi-Fi) não bloqueiem operações críticas de tempo real (como a aquisição de dados do sensor e a inferência do modelo).

mpu6500_task: Sua única função é coletar dados do acelerômetro MPU6500 na frequência de 60 Hz estipulada no requisito RF01. Ela opera com alta prioridade para garantir a precisão temporal da amostragem.

ML_task: É o núcleo de inteligência do sistema. Recebe os dados brutos da mpu6500_task, realiza o pré-processamento (RF02) e executa a inferência do modelo TinyML (RF03) para classificar o movimento. Esta tarefa é projetada para ser computacionalmente intensiva, mas sua execução em paralelo garante que a latência de inferência seja mantida abaixo de 200 ms (RNF02).

wifi_task: Gerencia a conectividade Wi-Fi. Sua responsabilidade é estabelecer e manter a conexão com o ponto de acesso.

mqtt_task: Responsável por transmitir os resultados da classificação para um broker MQTT na nuvem, atendendo ao requisito RF05. Ela formata a mensagem com a classe detectada e uma estampa de tempo, utilizando QoS 2 para garantir a entrega confiável da mensagem.

display_gate_task: Controla a exibição das informações no display OLED SSD1306 (RF04). Funciona como um "gatekeeper" para o periférico, evitando conflitos de acesso e desacoplando a lógica de exibição do processamento principal.

Mecanismos de Sincronização e Comunicação Inter-Tarefas

Para garantir a operação correta e a integridade dos dados, a comunicação entre as tarefas é gerenciada por mecanismos específicos do FreeRTOS:

Semáforo (entre wifi_task e mqtt_task): Conforme definido no diagrama de blocos, um semáforo atua como um mecanismo de sinalização. A mqtt_task deve aguardar um sinal (a "liberação" do semáforo) da wifi_task antes de tentar se conectar ao broker e enviar dados. Isso garante que a transmissão MQTT só seja iniciada após a confirmação de uma conexão Wi-Fi ativa, prevenindo falhas e otimizando recursos.

Fila (para a display_gate_task): A ML_task, após cada inferência, envia o resultado (a classe do movimento) para uma fila (Queue). A display_gate_task fica bloqueada aguardando a chegada de um item nessa fila. Essa estrutura desacopla o processamento da exibição: a ML_task não precisa esperar a lenta atualização do display I2C para continuar seu trabalho. A fila também serializa o acesso ao display, garantindo que as informações sejam exibidas de forma ordenada e sem corrupção.

Lógica de Baixo Consumo (Atendendo RNF03)

A lógica para atingir a autonomia de 30 dias é um pilar central do sistema. Ela é implementada na ML_task e atende ao requisito RF06.
Quando a inferência resulta na classe "Parado", a tarefa inicia um protocolo de economia de energia.
Este protocolo pode incluir a redução da frequência de amostragem da mpu6500_task (já que não são esperadas mudanças bruscas) e a desativação temporária do módulo Wi-Fi, que é um dos maiores consumidores de energia.
O sistema permanecerá neste estado de baixo consumo até que o acelerômetro detecte uma vibração que justifique reativar o modo de operação normal, garantindo que o dispositivo economize bateria significativamente durante os longos períodos de espera do contêiner.
Em suma, a estrutura proposta utiliza os pontos fortes do FreeRTOS para criar um sistema embarcado que é ao mesmo tempo reativo, eficiente e robusto, alinhado com todos os objetivos e requisitos definidos para o projeto.

_________________________________________________________________________________________

## Próxima etapa

Construir e testar o protótipo funcional, identificando ajustes necessários.

