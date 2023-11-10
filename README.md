Primeiramente irei explicar a arquitetura Básica para projetos IOT, em seguida apresentar do que o projeto é composto e por último irei explicar o código passo a passo. 

ARQUITETURA BÁSICA PARA PROJETOS DE IOT
Aplicação atua como a face visível da aplicação, responsável pelo front-end e pelas ferramentas que interagem com dispositivos de IoT e os consumidores/provedores de contexto. Além disso, ela é a casa de inovações tecnológicas como algoritmos de machine learning, inteligência artificial, análises avançadas, dashboards e aplicativos móveis.

Back-end é o núcleo operacional da plataforma, onde residem elementos vitais como o Orion Context Broker, STH-Comet, IoT Agent MQTT, e o banco de dados NoSQL MongoDB. Esses componentes desempenham um papel fundamental no armazenamento e gerenciamento de entidades, registros, subscrições e dados históricos (time series). Para facilitar a comunicação, também encontramos o Eclipse-Mosquitto, um popular broker MQTT. Além desses componentes-chave, a Camada de Back-end é um hub para a integração de outros GEs oferecidos pela FIWARE Foundation e ferramentas de terceiros. Esta camada não só proporciona a infraestrutura robusta para suportar a troca de informações, mas também serve como a espinha dorsal para a construção de soluções complexas e escaláveis.

IoT (Internet of Things) é responsável por acomodar os dispositivos de IoT que estabelecem comunicação com o back-end através dos protocolos de comunicação MQTT ou HTTP/NGSIv2.
![image](https://github.com/GDPMg/Sprint4-EDGE/assets/103905620/ea31ee72-ae7e-41e7-8931-a95928bb2371)


O Projeto é composto por: 1 ESP32, 1 LCD 16x2, 1 Botão, 1 LED, 1 Buzzer, 2 Resistores de 10K e 1 Resistor de de 220.
