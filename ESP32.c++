#include <WiFi.h>
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient
#include <LiquidCrystal.h> 

//defines:
//defines de id mqtt e tópicos para publicação e subscribe denominado TEF(Telemetria e Monitoramento de Equipamentos)
#define TOPICO_SUBSCRIBE    "/TEF/lamp104/cmd"        //tópico MQTT de escuta
#define TOPICO_PUBLISH      "/TEF/lamp104/attrs"      //tópico MQTT de envio de informações para Broker   
#define TOPICO_PUBLISH_3    "/TEF/lamp104/attrs/botao"
                                                      //IMPORTANTE: recomendamos fortemente alterar os nomes
                                                      //            desses tópicos. Caso contrário, há grandes
                                                      //            chances de você controlar e monitorar o ESP32
                                                      //            de outra pessoa.
#define ID_MQTT  "fiware_104"   //id mqtt (para identificação de sessão)
                                 //IMPORTANTE: este deve ser único no broker (ou seja, 
                                 //            se um client MQTT tentar entrar com o mesmo 
                                 //            id de outro já conectado ao broker, o broker 
                                 //            irá fechar a conexão de um deles).
                                 // o valor "n" precisa ser único!

// Define o pino do LED
const int ledPin = 2; // Pino GPIO 2 (ou outra opção)

// Define o pino do botão
const int buttonPin = 4; // Pino GPIO 4 (ou outra opção)

// Variáveis para contar o número de vezes que o botão foi pressionado
int contadorBotao = 0;
// Variáveis para rastrear o tempo
unsigned long ultimaPressao = 0;
unsigned long tempoParaRedefinir = 24 * 60 * 60 * 1000; // 24 horas em milissegundos

// Define o pino do buzzer
const int buzzerPin = 23; // Pino GPIO 23 (ou outra opção)

// Define os pinos do display LCD
const int rsPin = 12; // Pino GPIO 12 (ou outra opção)
const int enPin = 27; // Pino GPIO 27 (outra opção)
const int d4Pin = 26; // Pino GPIO 26 (outra opção)
const int d5Pin = 25; // Pino GPIO 25 (outra opção)
const int d6Pin = 33; // Pino GPIO 17 (outra opção)
const int d7Pin = 32; // Pino GPIO 16 (outra opção)

// Cria o objeto LiquidCrystal
LiquidCrystal lcd(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin);                               

// WIFI
const char* SSID = "Wokwi-GUEST"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = ""; // Senha da rede WI-FI que deseja se conectar
  
// MQTT
const char* BROKER_MQTT = "46.17.108.113"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
 
int D4 = 2;

//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
char EstadoSaida = '0';  //variável que armazena o estado atual da saída
  
//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi(); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
void InitOutput(void);
 
/* 
 *  Implementações das funções
 */
void setup() 
{
    //inicializações:
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();
    delay(5000);
    MQTT.publish(TOPICO_PUBLISH, "s|off");

      // Define o pino do LED como saída
  pinMode(ledPin, OUTPUT);
  
  // Define o pino do botão como entrada
  pinMode(buttonPin, INPUT);

  // Define o pino do buzzer como saída
  pinMode(buzzerPin, OUTPUT);

  // Inicializa o display LCD
  lcd.begin(16, 2);

      // Inicialize as variáveis de tempo
  ultimaPressao = millis();
}
  
//Função: inicializa comunicação serial com baudrate 115200 (para fins de monitorar no terminal serial 
//        o que está acontecendo.
//Parâmetros: nenhum
//Retorno: nenhum
void initSerial() 
{
    Serial.begin(115200);
}
 
//Função: inicializa e conecta-se na rede WI-FI desejada
//Parâmetros: nenhum
//Retorno: nenhum
void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
     
    reconectWiFi();
}
  
//Função: inicializa parâmetros de conexão MQTT(endereço do 
//        broker, porta e seta função de callback)
//Parâmetros: nenhum
//Retorno: nenhum
void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}
  
//Função: função de callback 
//        esta função é chamada toda vez que uma informação de 
//        um dos tópicos subescritos chega)
//Parâmetros: nenhum
//Retorno: nenhum
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
    String msg;
     
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);
    
    //toma ação dependendo da string recebida:
    //verifica se deve colocar nivel alto de tensão na saída D0:
    //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
    //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)
    if (msg.equals("lamp104@on|"))
    {
        digitalWrite(D4, HIGH);
        EstadoSaida = '1';
    }

    //verifica se deve colocar nivel alto de tensão na saída D0:
    if (msg.equals("lamp104@off|"))
    {
        digitalWrite(D4, LOW);
        EstadoSaida = '0';
    }
}
  
//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//        em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
//Parâmetros: nenhum
//Retorno: nenhum
void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}
  
//Função: reconecta-se ao WiFi
//Parâmetros: nenhum
//Retorno: nenhum
void reconectWiFi() 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}
 
//Função: verifica o estado das conexões WiFI e ao broker MQTT. 
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.
//Parâmetros: nenhum
//Retorno: nenhum
void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
     
     reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
 
//Função: envia ao Broker o estado atual do output 
//Parâmetros: nenhum
//Retorno: nenhum
void EnviaEstadoOutputMQTT(void)
{
    if (EstadoSaida == '1')
    {
      MQTT.publish(TOPICO_PUBLISH, "s|on");
      Serial.println("- Led Ligado");
    }
    if (EstadoSaida == '0')
    {
      MQTT.publish(TOPICO_PUBLISH, "s|off");
      Serial.println("- Led Desligado");
    }
    Serial.println("- Estado do LED onboard enviado ao broker!");
    delay(1000);
}
 
//Função: inicializa o output em nível lógico baixo
//Parâmetros: nenhum
//Retorno: nenhum
void InitOutput(void)
{
    //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
    //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);
    
    boolean toggle = false;

    for(int i = 0; i <= 10; i++)
    {
        toggle = !toggle;
        digitalWrite(D4,toggle);
        delay(200);           
    }

    digitalWrite(D4, LOW);
}
 
 
//programa principal
void loop() 
{   
    const int potPin = 34;
    
    char msgBuffer[4];
    //garante funcionamento das conexões WiFi e ao broker MQTT
    VerificaConexoesWiFIEMQTT();

    //envia o status de todos os outputs para o Broker no protocolo esperado
    EnviaEstadoOutputMQTT();
 
    // Verifica se o botão está pressionado
    if (digitalRead(buttonPin) == HIGH) {
        // Publica no tópico do botão
        MQTT.publish(TOPICO_PUBLISH_3, "pressionado");

        // Verifica se passou o tempo para redefinir a contagem
        if (millis() - ultimaPressao >= tempoParaRedefinir) {
            // Redefine o contador após 24 horas
            contadorBotao = 1;
        } else {
            // Incrementa o contador
            contadorBotao++;
        }

        // Atualiza o tempo da última pressão
        ultimaPressao = millis();
    } 
    else {
        // Publica no tópico do botão
        MQTT.publish(TOPICO_PUBLISH_3, "nao pressionado");

        // Resto do seu código

        // Publica o contador nas últimas 24 horas
        char contadorStr[10];
        itoa(contadorBotao, contadorStr, 10);
        MQTT.publish(TOPICO_PUBLISH_3, contadorStr);
    }
    // Verifica se o botão está pressionado
    if (digitalRead(buttonPin) == HIGH) {
      // Exibe a mensagem "Deficiente" na primeira linha
      lcd.setCursor(0, 0);
      lcd.print("Deficiente");
  
      // Exibe a mensagem "Fisico" na segunda linha
      lcd.setCursor(0, 1);
      lcd.print("Fisico");
  
      // Pisca o LED três vezes
      for (int i = 0; i < 3; i++) {
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
      }
  
      // Toca o som no buzzer
      tone(buzzerPin, 300, 500);
  
      // Espera 1 segundo antes de tocar a buzina
      delay(1000);
    } else {
      // Para o LED e o buzzer se o botão não estiver pressionado
      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
  
      // Limpa o display LCD
      lcd.clear();
    }

    //keep-alive da comunicação com broker MQTT
    MQTT.loop();
}
