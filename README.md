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
