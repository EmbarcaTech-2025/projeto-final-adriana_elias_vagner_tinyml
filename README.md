<h1 align = "center"> Sistema para Monitoramento Inteligente de Movimentos em Contêineres Marítimos com TinyML </h1>

<h2 align = "center"> Projeto: Embarcatech - Fase 2 </h2>
<h3 align = "center"> Instituto Hardware BR - HBr </h3>
<h3 align = "center"> Campinas-SP </h3>

----
### Time:
- Adriana R. Castro de Paula
- Elias Kento Tomiyama
- Vagner Sanches Vasconcelos
----

## **1\. Resumo do Projeto**

Este projeto apresenta o desenvolvimento de um sistema embarcado de baixo custo para a **classificação de movimentos de contêineres marítimos** em tempo real. Utilizando a plataforma **BitDogLab** (RP2040), um sensor acelerômetro (MPU6050) e a tecnologia **Tiny Machine Learning (TinyML)**, o sistema identifica quatro classes de movimento: "Parado", "Subindo/Descendo", "Esquerda/Direita" e "Ziguezague".

Os resultados da classificação são exibidos localmente em um display OLED e enviados via Wi-Fi para um broker **MQTT**, permitindo o monitoramento remoto e a otimização da logística. Todo o sistema é orquestrado pelo **FreeRTOS**, garantindo uma execução robusta e concorrente das tarefas.


## **2\. Contexto do Problema e Solução Proposta**

A logística global de contêineres enfrenta desafios no rastreamento e na identificação de eventos específicos, como manuseio incorreto ou anomalias durante o transporte. Os métodos tradicionais, como GPS e registros manuais, não oferecem a granularidade necessária para entender o tipo de movimento.

Para solucionar este desafio, foi desenvolvido um sistema embarcado que confere "inteligência" ao próprio contêiner. A solução se baseia em três pilares principais:

1.  **Sensor de Aceleração (MPU6050):** Captura os dados de movimento do contêiner.
2.  **TinyML na Borda:** Um modelo de Machine Learning, treinado no **Edge Impulse**, executa a inferência diretamente no microcontrolador para analisar os dados do sensor em tempo real, sem a necessidade de uma conexão constante com a nuvem.
3.  **Conectividade IoT (Wi-Fi e MQTT):** Os resultados são transmitidos para uma plataforma de monitoramento remoto, permitindo que operadores logísticos e gestores de frota tenham visibilidade completa da movimentação do contêiner.
