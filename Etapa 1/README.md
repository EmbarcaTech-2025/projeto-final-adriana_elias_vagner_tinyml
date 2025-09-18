<h1 align = "center"> Classificação de Movimentos em Contêineres Marítimos com TinyML na BitDogLab </h1>

<h2 align = "center"> Projeto: Embarcatech - Fase 2 </h2>
<h3 align = "center"> Instituto Hardware BR - HBr </h3>
<h3 align = "center"> Campinas-SP </h3>

----
### Time:
- Adriana R. Castro de Paula
- Elias Kento Tomiyama
- Vagner Sanches Vasconcelos
----

# 1. Contexto - Problema
A logística de contêineres marítimos é um componente crítico da cadeia de suprimentos global, envolvendo etapas como movimentação por empilhadeiras ou guindastes, transporte terrestre em caminhões e transporte marítimo em navios. A rastreabilidade precisa dessas etapas é essencial para otimizar rotas, reduzir custos operacionais, prevenir danos causados por vibrações excessivas e detectar anomalias em tempo real. No entanto, sistemas tradicionais baseados em GPS e registros manuais carecem de granularidade para identificar o tipo de movimento do contêiner, limitando a capacidade de resposta proativa.

# 2. Usuários
O sistema proposto oferece utilidade direta para múltiplos atores da cadeia logística, gerando valor econômico e operacional.
**Os usuários-alvo do sistema incluem:**
- **Operadores logísticos:** Gerenciam cadeias de suprimentos e se beneficiam de dados em tempo real para otimizar rotas e reduzir custos (a automação do registro de etapas (ex: início do transporte terrestre) elimina a necessidade de apontamentos manuais, reduzindo erros e custos administrativos).
- **Gestores de armazéns e portos:** Necessitam de monitoramento preciso para prevenir danos durante o manuseio e melhorar a segurança (a detecção de padrões de vibração anômalos pode indicar manuseio incorreto ou tentativa de violação, disparando alertas imediatos).
- **Desenvolvedores de sistemas embarcados:** Podem usar o projeto como base para soluções escaláveis, integrando novos sensores ou funcionalidades.
- **Empresas de transporte marítimo:** Interessadas em rastreabilidade detalhada para detectar anomalias e melhorar a eficiência operacional.
- **Para o Ecossistema de Cidades Inteligentes:** A coleta de dados em massa sobre o fluxo de contêineres pode alimentar sistemas de gerenciamento de tráfego e planejamento urbano, otimizando as rotas de veículos pesados.

TinyML permite a execução de modelos de aprendizado de máquina em microcontroladores com recursos limitados, como o RP2040 da plataforma BitDogLab. Este projeto propõe um sistema embarcado que utiliza o acelerômetro MPU6500, a plataforma Edge Impulse<sup>2</sup> para treinamento de modelos, e o protocolo MQTT para transmissão de dados, classificando automaticamente os movimentos de contêineres em quatro classes:
> (i) Parado (armazenado ou em espera);
> (ii) Subindo/descendo (movimentação por empilhadeira ou guindaste);
> (iii) Esquerda/direita (transporte terrestre); e
> (iv) Ziguezague (transporte marítimo).

O sistema opera com baixo consumo energético, exibe resultados localmente em um display SSD1306 e transmite dados via Wi-Fi, alinhando-se aos objetivos do curso Embarcatech de criar soluções embarcadas inovadoras para desafios reais.
A figura 1, ilustra o estudo de caso em questão.
![diagrama](/assets/estudoCaso.png)
<h4 align = "right"> Figura 1 - Caso de Uso. Fonte: Curso UNIFEI-IESTI01- TinyML </h4>

# 3. Requisitos
## 3.1 Requisitos Funcionais

- RF01: Coletar dados do acelerômetro MPU6500 (eixos X, Y, Z) a 60Hz;
- RF02: Pré-processar os dados (filtro passa-baixa, normalização e janelamento);
- RF03: Executar modelo TinyML para classificação em tempo real;
- RF04: Exibir a classe detectada no display SSD1306;
- RF05: Transmitir resultados via Wi-Fi para um broker MQTT em nuvem, com QoS 2 e estampa de tempo (dia/mês/ano - hora:minuto: segundo);
- RF06: Operar em modo de baixo consumo quando parado;
- RF07: Utilizar Sistema Operacional de Tempo Real (RTOS).

## 3.2 Requisitos Não Funcionais

- RNF01: Taxa de acerto ≥ 90% em condições reais (dados de testes);
- RNF02: Latência de inferência ≤ 200ms;
- RNF03: Autonomia mínima de 30 dias com bateria;
- RNF04: Funcionamento estável em temperaturas de 0°C a 50°C;
- RNF05:  Código modular e documentado para manutenibilidade.

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

___________________________________________________________________________________________

## Próxima etapa

Desenvolvimento da arquitetura do sistema, com início da prototipagem, integrando sensores com a lógica embarcada e interface de comunicação.
