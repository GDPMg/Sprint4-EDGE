Primeiramente irei explicar a arquitetura Básica para projetos IOT, em seguida apresentar do que o projeto é composto e por último irei explicar o código passo a passo. 

ARQUITETURA BÁSICA PARA PROJETOS DE IOT
Aplicação atua como a face visível da aplicação, responsável pelo front-end e pelas ferramentas que interagem com dispositivos de IoT e os consumidores/provedores de contexto. Além disso, ela é a casa de inovações tecnológicas como algoritmos de machine learning, inteligência artificial, análises avançadas, dashboards e aplicativos móveis.

Back-end é o núcleo operacional da plataforma, onde residem elementos vitais como o Orion Context Broker, STH-Comet, IoT Agent MQTT, e o banco de dados NoSQL MongoDB. Esses componentes desempenham um papel fundamental no armazenamento e gerenciamento de entidades, registros, subscrições e dados históricos (time series). Para facilitar a comunicação, também encontramos o Eclipse-Mosquitto, um popular broker MQTT. Além desses componentes-chave, a Camada de Back-end é um hub para a integração de outros GEs oferecidos pela FIWARE Foundation e ferramentas de terceiros. Esta camada não só proporciona a infraestrutura robusta para suportar a troca de informações, mas também serve como a espinha dorsal para a construção de soluções complexas e escaláveis.

IoT (Internet of Things) é responsável por acomodar os dispositivos de IoT que estabelecem comunicação com o back-end através dos protocolos de comunicação MQTT ou HTTP/NGSIv2.

![image](https://github.com/GDPMg/Sprint4-EDGE/assets/103905620/ea31ee72-ae7e-41e7-8931-a95928bb2371)

O PROJETO É COMPOSTO POR: 
1 ESP32, 1 LCD 16x2, 1 Botão, 1 LED, 1 Buzzer, 2 Resistores de 10K e 1 Resistor de de 220.

![image](https://github.com/GDPMg/Sprint4-EDGE/assets/103905620/27dbc57d-3d32-471b-9619-363f5812f0d0)

EXPLICANDO O CÓDIGO:
BIBLIOTECAS INCLUÍDAS:
                                                #include <WiFi.h>
                                                #include <PubSubClient.h>
                                                #include <LiquidCrystal.h>

Aqui, está incluindo as bibliotecas necessárias para trabalhar com Wi-Fi, MQTT (usando o PubSubClient) e um display LCD.

DEFINIÇÕES E CONSTANTES:
                                                #define TOPICO_SUBSCRIBE    "/TEF/lamp104/cmd"
                                                #define TOPICO_PUBLISH      "/TEF/lamp104/attrs"
                                                // ... outras definições ...

                                                const int ledPin = 2;
                                                const int buttonPin = 4;
                                                const int buzzerPin = 23;
                                                // ... outras constantes ...
                                                
Aqui, está definindo tópicos MQTT, pinos GPIO, e outras constantes que serão utilizadas no programa.

INICIALIZAÇÃO DE HARDWARE:
                                                LiquidCrystal lcd(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin);
                                                
Aqui, está criando um objeto LiquidCrystal para interagir com o display LCD.

CONFIGURAÇÃO DE WI-FI E MQTT:
                                                const char* SSID = "Wokwi-GUEST";
                                                const char* PASSWORD = "";
                                                const char* BROKER_MQTT = "46.17.108.113";
                                                int BROKER_PORT = 1883;
                                                
Estas são as configurações de Wi-Fi e MQTT. O código conecta o ESP32 à rede Wi-Fi e ao broker MQTT.

CALLBACKS MQTT:
                                                void mqtt_callback(char* topic, byte* payload, unsigned int length) {
                                                    // ... lógica para processar mensagens recebidas ...
                                                } 
                                                
Este é um callback chamado sempre que uma mensagem MQTT é recebida no tópico inscrito.

FUNÇÃO PRINCIPAL (SETUP e LOOP):
                                                void setup() {
                                                    // ... inicializações ...
                                                }
                                                
                                                void loop() {
                                                    // ... lógica principal do programa ...
                                                }
                                                
Aqui, tem as funções setup e loop. A função setup é executada uma vez no início, enquanto a função loop é executada repetidamente.


COMUNICAÇÃO MQTT E WI-FI:
                                                void VerificaConexoesWiFIEMQTT(void) {
                                                    // ... verifica e reconecta-se ao Wi-Fi e MQTT se necessário ...
                                                }

                                                void EnviaEstadoOutputMQTT(void) {
                                                    // ... envia o estado atual para o broker MQTT ...
                                                }
                                                
Estas funções tratam da conexão e envio de mensagens MQTT.

MANIPULÇÃO DE BOTÃO E CONTADOR:
                                                // ... lógica para verificar o estado do botão e manipular um contador ...
                                                
Esta parte do código verifica se um botão está pressionado, incrementa um contador e envia a informação via MQTT.

CONTROLE DE SAÍDA (LED):
                                                void InitOutput(void) {
                                                    // ... inicializa o estado do LED ...
                                                }
                                                
Esta função controla a saída do LED, considerando uma lógica invertida.

DISPLAY LCD E OUTRAS FUNCIONALIDADES:
                                                // ... lógica para exibir mensagens no display LCD e controlar um buzzer ...
                                                
Aqui, você tem a lógica para exibir mensagens em um display LCD e controlar um buzzer com base no estado do botão.

LOOP PRINCIPAL:
                                                void loop() {
                                                    // ... lógica principal do programa (verificações, envios MQTT, etc.) ...
                                                }
O loop principal do programa, onde a maioria da lógica acontece.

Este código é bastante abrangente e possui funcionalidades relacionadas à comunicação MQTT, controle de saída, manipulação de botões, exibição em um display LCD e controle de um buzzer. O código é destinado a um sistema embarcado que utiliza um ESP32 para comunicação e controle.


