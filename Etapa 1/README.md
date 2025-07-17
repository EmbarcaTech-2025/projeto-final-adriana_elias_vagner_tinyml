<h1 align = "center"> Classificação de Movimentos em Contêineres Marítimos com TinyML na BitDogLab </h1> 

<h2 align = "center"> Projeto: Embarcatech - Fase 2 </h2>   

----
- Adriana R. Castro de Paula    
- Elias Kento Tomiyama    
- Vagner Sanches Vasconcelos     
----

# 1. Contexto
A logística de contêineres marítimos é um componente crítico da cadeia de suprimentos global, envolvendo etapas como movimentação por empilhadeiras ou guindastes, transporte terrestre em caminhões e transporte marítimo em navios. A rastreabilidade precisa dessas etapas é essencial para otimizar rotas, reduzir custos operacionais, prevenir danos causados por vibrações excessivas e detectar anomalias em tempo real. No entanto, sistemas tradicionais baseados em GPS e registros manuais carecem de granularidade para identificar o tipo de movimento do contêiner, limitando a capacidade de resposta proativa.    

A **concepção** deste projeto é criar um sistema embarcado de baixo custo e alta autonomia, embarcado no próprio contêiner, que utilize sensores inerciais e inteligência artificial na borda (edge) para resolver esse problema. A ideia é "dar inteligência" ao contêiner, permitindo que ele "entenda" seu próprio estado de movimento e comunique essa informação em tempo real. A solução se baseia no campo emergente de Tiny Machine Learning (TinyML), que foca na implementação de modelos de aprendizado de máquina em microcontroladores com recursos limitados.    

TinyML permite a execução de modelos de aprendizado de máquina em microcontroladores com recursos limitados, como o RP2040 da plataforma BitDogLab. Este projeto propõe um sistema embarcado que utiliza o acelerômetro MPU6500, a plataforma Edge Impulse para treinamento de modelos, e o protocolo MQTT para transmissão de dados, classificando automaticamente os movimentos de contêineres em quatro classes: (i) parado (armazenado ou em espera), (ii) subindo/descendo (movimentação por empilhadeira ou guindaste), (iii) esquerda/direita (transporte terrestre), e (iv) ziguezague (transporte marítimo). O sistema opera com baixo consumo energético, exibe resultados localmente em um display SSD1306 e transmite dados via Wi-Fi, alinhando-se aos objetivos do curso Embarcatech de criar soluções embarcadas inovadoras para desafios reais.  

# 2. Usuários
O sistema proposto oferece utilidade direta para múltiplos atores da cadeia logística, gerando valor econômico e operacional.    
**Os usuários-alvo do sistema incluem:**    
- **Operadores logísticos:** Gerenciam cadeias de suprimentos e se beneficiam de dados em tempo real para otimizar rotas e reduzir custos (a automação do registro de etapas (ex: início do transporte terrestre) elimina a necessidade de apontamentos manuais, reduzindo erros e custos administrativos).      
- **Gestores de armazéns e portos:** Necessitam de monitoramento preciso para prevenir danos durante o manuseio e melhorar a segurança (a detecção de padrões de vibração anômalos pode indicar manuseio incorreto ou tentativa de violação, disparando alertas imediatos).
- **Desenvolvedores de sistemas embarcados:** Podem usar o projeto como base para soluções escaláveis, integrando novos sensores ou funcionalidades.    
- **Empresas de transporte marítimo:** Interessadas em rastreabilidade detalhada para detectar anomalias e melhorar a eficiência operacional.    
- **Para o Ecossistema de Cidades Inteligentes:** A coleta de dados em massa sobre o fluxo de contêineres pode alimentar sistemas de gerenciamento de tráfego e planejamento urbano, otimizando as rotas de veículos pesados.  

# 3. Objetivo Geral  
Desenvolver um sistema embarcado baseado na plataforma BitDogLab, utilizando TinyML e o acelerômetro MPU6500, para classificar automaticamente os movimentos de contêineres marítimos em quatro classes (parado, subindo/descendo, esquerda/direita, ziguezague), com visualização local em um display SSD1306, transmissão de dados via MQTT para monitoramento remoto, e operação otimizada para baixo consumo energético, alcançando autonomia mínima de 30 dias.

# 4. Objetivos Específicos
- 1.Coletar dados do acelerômetro MPU6500 (eixos X, Y, Z) a 60 Hz para capturar padrões de movimento.    
- 2.Pré-processar os dados com filtro passa-baixa, normalização e janelamento para preparar os sinais para o modelo TinyML.    
- 3.Desenvolver, treinar e avaliar um modelo de aprendizado de máquina na plataforma Edge Impulse, alcançando acurácia superior a 80%.    
- 4.Implementar o modelo TinyML na BitDogLab para inferência em tempo real com latência inferior a 200 ms.    
- 5.Exibir a classe de movimento detectada em um display OLED SSD1306.   
- 6.Transmitir os resultados via Wi-Fi para um broker MQTT com qualidade de serviço (QoS) 2 e e estampa de tempo (dia/mês/ano - hora:minuto: segundo);
- 7.Garantir operação em modo de baixo consumo para autonomia mínima de 30 dias com bateria.   
- 8.Utilizar FreeRTOS para gerenciar tarefas concorrentes, assegurando estabilidade e escalabilidade.    

# 5. Requisitos
## 5.1 Requisitos Funcionais

- RF01: Coletar dados do acelerômetro MPU6500 (eixos X, Y, Z) a 60Hz;     
- RF02: Pré-processar os dados (filtro passa-baixa, normalização e janelamento);     
- RF03: Executar modelo TinyML para classificação em tempo real;     
- RF04: Exibir a classe detectada no display SSD1306;     
- RF05: Transmitir resultados via Wi-Fi para um broker MQTT em nuvem, com QoS 2 e estampa de tempo (dia/mês/ano - hora:minuto: segundo);     
- RF06: Operar em modo de baixo consumo quando parado;     
- RF07: Utilizar Sistema Operacional de Tempo Real (RTOS).    

## 5.2 Requisitos Não Funcionais 

- RNF01: Taxa de acerto ≥ 90% em condições reais (dados de testes);  
- RNF02: Latência de inferência ≤ 200ms;  
- RNF03: Autonomia mínima de 30 dias com bateria;  
- RNF04: Funcionamento estável em temperaturas de 0°C a 50°C;  
- RNF05:  Código modular e documentado para manutenibilidade. 

# 6. Abordagem
O projeto segue a metodologia CUGNASA, estruturada em quatro etapas, com atividades divididas em duas frentes: desenvolvimento do sistema embarcado e desenvolvimento do modelo TinyML. A abordagem é iterativa, com ciclos de projeto, prototipagem, teste e refinamento, utilizando ferramentas como Pico-SDK, FreeRTOS e Edge Impulse.   

## 6.1 Etapa 1: Definição de Requisitos e Lista de Materiais (Entrega: 17/07/2025)  
**Objetivo:** Consolidar o problema, definir requisitos e listar materiais necessários.   
**Atividades:**   
- **Documentando o problema:** O rastreamento de contêineres carece de granularidade para identificar tipos de movimento, impactando eficiência e segurança. A solução baseada em TinyML e IoT permite monitoramento detalhado e em tempo real.   
- **Detalhando requisitos:** Definir RFs e RNFs com base no contexto logístico, garantindo alinhamento com as necessidades dos usuários.    
- **Listando materiais:**    
    - Hardware:
        - BitDogLab (RP2040): Microcontrolador principal.    
        - MPU6500: Acelerômetro de 6 eixos (I2C).   
        - SSD1306: Display OLED 128x64 (I2C).     
        - Bateria: LiPo 3.7V, capacidade ~2000 mAh (a ser confirmada).   
    - Software:   
        - Pico-SDK: Framework para programação do RP2040.   
        - FreeRTOS: Gerenciamento de tarefas concorrentes.   
        - Edge Impulse: Desenvolvimento do modelo TinyML.   
        - Paho MQTT: Biblioteca para comunicação MQTT.   
- **Estudando integração:** Analisar a conexão dos periféricos (I2C para MPU6500 e SSD1306, SPI para Wi-Fi) e compatibilidade com o RP2040.      
- **Entregável:** Este documento em Markdown, contendo a descrição do problema, requisitos técnicos e lista de materiais.   

## 6.2 Etapa 2: Arquitetura e Modelagem (Entrega: 04/08/2025)
**Objetivo:** Definir a arquitetura do sistema e modelar o software e o modelo TinyML.   
**Atividades:**  
- Desenvolvendo diagrama de hardware:  
    - Conexões:   
        1.MPU6500: I2C (SDA: GPIO4, SCL: GPIO5).   
        2.SSD1306: I2C (mesmo barramento, endereços distintos).      
        3.Bateria: Conectada ao regulador de tensão da BitDogLab.    

- Diagrama de software:    
![diagrama](etapa_1_diagrama.png)  

    - **Criando fluxograma do software:**   
        - **Inicialização:** Configurar FreeRTOS, inicializar periféricos (MPU6500, SSD1306, Wi-Fi).   
        - **Loop Principal:**   
            1.Ler dados do MPU6500 (60 Hz).   
            2.Pré-processar (filtro passa-baixa, normalização, janelamento).   
            3.Executar inferência TinyML.   
            4.Exibir classe no SSD1306.   
            5.Transmitir via MQTT.   
            6.Verificar estado parado para modo de baixo consumo.    

    - **Configurando ambiente:**    
        - Instalar Pico-SDK e configurar FreeRTOS no RP2040.    
        - Configurar Edge Impulse para coleta e treinamento de dados.    

    - **Definindo arquitetura do modelo TinyML:**    
        - Pré-processamento;        
        - Modelo: Rede neural densa;   
        - Saída: Probabilidades para as classes: parado, subindo/descendo, esquerda/direita, ziguezague.   
- **Entregável:** Arquivo com diagramas de hardware e software, acompanhado de explicações detalhando a lógica e estrutura do sistema.   

## 6.3 Etapa 3: Prototipagem e Ajustes (Entrega: 25/08/2025)
**Objetivo:** Construir e testar o protótipo funcional, identificando ajustes necessários.   
**Atividades:**   
- **Sistema Embarcado:**   
    - **Configurando FreeRTOS:** Implementar tarefas concorrentes;        
    - **Implementando aquisição de dados:** Configurar MPU6500 via I2C, ajustando escala (±2g) e frequência (60 Hz).   
    - **Configurando display:** Inicializar SSD1306 para exibir a classe detectada (ex.: "Parado", "Subindo", etc.).   
    - **Implementando MQTT:** Configurar cliente Paho MQTT, conectar ao broker (ex.: Mosquitto) e publicar mensagens com QoS 2.   
    - **Otimizando energia:** Ativar modo sleep do RP2040 quando parado, reduzindo frequência de amostragem e desativando Wi-Fi.   

- **Modelo TinyML:**
    - **Coletando dados:**    
    - **Pré-processando no Edge Impulse:**
    - **Projetando modelo:**   
    - **Treinando e avaliando:**   
    - **Convertendo modelo:**   
    - **Executando inferência:** 
    - **Testes:**
    - **Métricas:**
    - **Consumo:** 
    - **Documentando desafios:** 
**Entregável:** Vídeo/fotos do protótipo, relatório detalhando testes, desafios e melhorias planejadas.

## 6.4 Etapa 4: Entrega Final e Documentação (Data a ser definida)
**Objetivo:** Finalizar o projeto com ajustes implementados e documentação completa.
**Atividades:**
- **Implementando ajustes:**
- **Otimizando energia:**
- **Finalizando documentação:**
- **Publicando no GitHub:**   
**Entregável:** Sistema funcional, documentação completa (relatório, diagramas, código), repositório GitHub.    

# 7. Solução
## 7.1 Hardware
- **BitDogLab (RP2040):** Microcontrolador principal, gerencia tarefas e comunicação.    
- **MPU6500:** Acelerômetro de 6 eixos (I2C), coleta dados a 60 Hz.    
- **SSD1306:** Display OLED 128x64 (I2C), exibe classe de movimento.    
- **Bateria:** LiPo 3.7V, 2000 mAh, para autonomia de 30 dias.    

## 7.2 Software
- **Pico-SDK:** Configuração de periféricos e comunicação.   
- **FreeRTOS:** Gerenciamento de tarefas concorrentes:      
- **Edge Impulse:** Desenvolvimento do modelo TinyML.    

## 7.3 Modelo TinyML


# 8. Avaliação
## 8.1 Critérios
- Clareza Técnica: Documentação detalhada com diagramas claros.
- Qualidade da Documentação: Código modular, comentado, README no GitHub.
- Criatividade e Aplicabilidade: Solução inovadora para logística, escalável.
- Complexidade e Domínio: Uso avançado da BitDogLab, TinyML, FreeRTOS, MQTT.
- Métricas:
    - Acurácia: > 80%.
    - Latência: < 200 ms.
    - Autonomia: ≥ 30 dias.
    - Estabilidade: 0°C a 50°C.

## 8.2 Plano de Testes
- Testes Unitários:
    - Aquisição de dados: Verificar frequência de 60 Hz.
    - Pré-processamento: Validar filtro e janelamento.
    - Exibição: Confirmar atualização do SSD1306.
    - Transmissão: Testar mensagens MQTT com QoS 2.
- Testes de Integração:
    - Verificar interação entre tarefas FreeRTOS.
    - Testar inferência com modelo integrado.
- Testes de Campo:
    - Simular movimentos (parado, elevação, deslocamento linear, oscilações).
    - Medir acurácia, latência e consumo.
- Testes de Estresse:
    - Operação por 48 horas em 0°C e 50°C.
    - Simular falhas Wi-Fi para verificar reconexão.

# 9. Cronograma
|Etapa|Objetivo|Entregável|Data|   
|---|---|---|---|  
|1|Definição de Requisitos e Materiais|Documento em Markdown|17/07/2025|    
|2|Arquitetura e Modelagem|Diagramas de hardware e software|04/08/2025|     
|3|Prototipagem e Ajustes|Vídeo/fotos do protótipo, relatório|25/08/2025|      
|4|Entrega Final e Documentação|Sistema funcional, documentação, GitHub|a definir|     

# 10. Considerações Finais
O projeto integra sistemas embarcados, TinyML e IoT para resolver um problema crítico na logística de contêineres, oferecendo rastreabilidade detalhada e eficiência energética. A metodologia CUGNASA garante clareza na definição do problema, alinhamento com usuários e uma abordagem estruturada. A solução tem potencial para impactar cadeias logísticas, com aplicações escaláveis em cenários industriais.

**Glossário**

[1]: TinyML: Tiny Machine Learning (TinyML) é um campo emergente da IA e da aprendizagem de máquina (ML) que se concentra no desenvolvimento e implantação de modelos de ML altamente otimizados em dispositivos de hardware com recursos extremamente limitados, como microcontroladores de baixo consumo energético. Esses dispositivos geralmente operam com restrições severas de memória, poder computacional e energia, muitas vezes na ordem de kilobytes de RAM, megahertz de clock e consumo de miliwatts ou menos.

[2]: Edge Impulse: É uma plataforma de desenvolvimento TinyML e machine learning para dispositivos de borda (edge) que permite a criação, treinamento e implantação de modelos de aprendizado de máquina otimizados para microcontroladores (MCUs) e sistemas embarcados de baixo consumo energético.

[3]: AIoT: Artificial Intelligence of Thing é a integração de IA com a IoT, combinando a capacidade de coleta de dados de dispositivos IoT com técnicas avançadas de processamento, como ML e deep learning (DL), para permitir análise em tempo real, tomada de decisão autônoma e otimização de sistemas embarcados.

**Referencias**
https://www.objective.com.br/insights/tinyml/

https://ichi.pro/pt/o-que-e-tinyml-e-por-que-isso-importa-270142243498102

https://www.geeksforgeeks.org/machine-learning/what-is-tinyml-tiny-machine-learning/

https://www.tinyml.com/

https://zlib.pub/book/tinyml-machine-learning-with-tensorflow-lite-on-arduino-and-ultra-low-power-microcontrollers-vshhregc28o0

https://github.com/dhruvmsheth/Oreilly-tinyml-book/blob/master/OReilly.TinyML.1492052043.pdf

https://edgeimpulse.com/

https://www.bing.com/videos/riverview/relatedvideo?q=edge+impulse&mid=7EEE774837E1F7D3C6837EEE774837E1F7D3C683&FORM=VIRE

https://www.bing.com/videos/riverview/relatedvideo?q=edge+impulse&mid=41C8C11509B60E3C360B41C8C11509B60E3C360B&FORM=VIRE

https://aidude.info/pt/ferramentas/edge-impulse

https://digital.futurecom.com.br/artigos/aiot-o-que-e-inteligencia-artificial-das-coisas/

https://www.bosch.com.br/noticias-e-historias/aiot/

https://www.bing.com/videos/riverview/relatedvideo?q=AIoT&mid=9671E43B51A222DB9F229671E43B51A222DB9F22&FORM=VIRE

https://cienciaembarcada.com.br/publicacoes/o-que-e-iot-iiot-e-aiot/


