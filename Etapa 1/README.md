# Classificação de Movimentos em Contêineres Marítimos com TinyML na BitDogLab

----
Adriana R. Castro de Paula    
Elias Kento Tomiyama    
Vagner Sanches Vasconcelos
----

# 1 Introdução

A logística de contêineres marítimos envolve diferentes etapas de transporte, incluindo movimentação por empilhadeiras, transporte terrestre em caminhões e transporte marítimo em navios. Identificar automaticamente em qual etapa um contêiner se encontra pode melhorar a rastreabilidade, segurança e eficiência operacional de sistemas logísticos.

Este projeto propõe um sistema embarcado baseado em TinyML<sup>1</sup>, utilizando a placa BitDogLab equipada com um acelerômetro MPU6500, para classificar movimentos em quatro classes, sendo elas:  i) Parado (armazenado ou espera); ii) Subindo/Descendo (movimentação por empilhadeira/guindastes); iii) Esquerda/Direita (transporte terrestre) ; e iv) Zig/Zag (transporte marítimo).

A classificação é realizada por um modelo de aprendizado de máquina (Machine Learning - ML) treinado na plataforma Edge Impulse<sup>2</sup> localmente (borda) na BitDogLab. Os resultados serão exibidos em um display SSD1306 e transmitidos via MQTT para um broker em nuvem, permitindo monitoramento remoto. 

# 2 Descrição do Problema

O rastreamento de contêineres marítimos muitas vezes depende de sistemas GPS e registros manuais, que não discriminam o tipo de movimento durante o transporte. Isso limita a capacidade de otimizar rotas, prevenir danos por vibrações excessivas ou detectar anomalias.  

## 2.1 Objetivo Geral  

Desenvolver um sistema embarcado capaz de classificar automaticamente o movimento de um contêiner em função dos sinais de acelerômetro em um modelo de machine learning embarcado na BitDogLab (AIoT<sup>3</sup>), permitindo: 

* Identificação em tempo real da etapa logística;  
* Visualização local no display;  
* Transmissão dos dados para análise remota via MQTT.  

O modelo de ML treinado deve detectar os padrões de aceleração, de forma a permitir a classificação (inferência) confiável entre as quatro classes de movimento definidas para o contêiner: i) Parado; ii) Movimento empilhadeira/guindastes; iii) Transporte terrestre ; e iv) Transporte marítimo.

# 3 Requisitos do Sistema

## 3.1 Requisitos Funcionais

RF01: Coletar dados do acelerômetro MPU6500 (eixos X, Y, Z) a 60Hz;  
RF02: Pré-processar os dados (filtro passa-baixa, normalização e janelamento);  
RF03: Executar modelo TinyML para classificação em tempo real;  
RF04: Exibir a classe detectada no display SSD1306;  
RF05: Transmitir resultados via Wi-Fi para um broker MQTT em nuvem, com QoS 2 e estampa de tempo (dia/mês/ano - hora:minuto: segundo);  
RF06: Operar em modo de baixo consumo quando parado;  
RF07: Utilizar Sistema Operacional de Tempo Real (RTOS).

## 3.2 Requisitos Não Funcionais 

RNF01: Taxa de acerto ≥ 90% em condições reais (dados de testes);  
RNF02: Latência de inferência ≤ 200ms;  
RNF03: Autonomia mínima de 30 dias com bateria;  
RNF04: Funcionamento estável em temperaturas de 0°C a 50°C;  
RNF05:  Código modular e documentado para manutenibilidade. 

# 4 Lista de Componentes e Materiais

## 4.1 Hardware  

- BitDogLab: Placa de Desenvolvimento;  
- Periférico: Placa com módulo MPU6500.  

## 4.2 Software e Ferramentas  

- Pico-SDK;  
- FreeRTOs;  
- Edge Impulse.

# 5 Diagrama 

![diagrama](etapa_1_diagrama.png)

# 6 Metodologia

O trabalho envolve duas grandes atividades, sendo elas;  
1ª) O desenvolvimento do sistema embarcado, com as configurações das tarefas no RTOS: i) Aquisição dos sinais do acelerômetro; ii) Conexão ao wi-fi; iii) Envio dos sinais por meio do protocolo mqttt; e  iv) Acesso ao display.    
Nesta atividade, serão utilizados o pico-sdk e o FreeRTOs; e  
2ª) O desenvolvimento do modelo de classificação de movimentos do contêiner, em função dos sinais do acelerômetro. Essa atividade ocorrerá na plataforma Edge Impulse, e, de forma geral, envolve as seguintes etapas: i) coleta dos dados;  ii) Pré Processamento dos dado; iii) Design do modelo de ML; iv) Treinamento do modelo de ML; v) Avaliação do modelo de ML; vi) Conversão do modelo de ML para a BitDogLab (RP2040); vii) Incorporação do modelo convertido na BitDogLab (Deploy); e viii) Inferência do modelo.

# 

[1]:  **TinyML**: Tiny Machine Learning (TinyML) é um campo emergente da IA e da aprendizagem de máquina (ML) que se concentra no desenvolvimento e implantação de modelos de ML altamente otimizados em dispositivos de hardware com recursos extremamente limitados, como microcontroladores de baixo consumo energético. Esses dispositivos geralmente operam com restrições severas de memória, poder computacional e energia, muitas vezes na ordem de kilobytes de RAM, megahertz de clock e consumo de miliwatts ou menos.

[2]:  **Edge Impulse**: É uma plataforma de desenvolvimento TinyML e machine learning para dispositivos de borda (edge) que permite a criação, treinamento e implantação de modelos de aprendizado de máquina otimizados para microcontroladores (MCUs) e sistemas embarcados de baixo consumo energético.

[3]:  **AIoT**: Artificial Intelligence of Thing é a integração de IA com a IoT, combinando a capacidade de coleta de dados de dispositivos IoT com técnicas avançadas de processamento, como ML e deep learning (DL), para permitir análise em tempo real, tomada de decisão autônoma e otimização de sistemas embarcados.
