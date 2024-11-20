Arduino.h: Biblioteca base do Arduino.
ESP8266WiFi.h: Fornece a funcionalidade de conexão Wi-Fi para o ESP8266.
ArduinoOTA.h: Biblioteca que permite realizar atualizações de firmware OTA.

# Aqui nessa linha de comando vamos definir as credencias da rede wi-fi
const char* ssid = "Gabriel";
const char* password = "1234567a";

void setup () {
  Serial.begin(9600);

 # Aqui temos uma taxa de trasnmissão de 9600 para vermos as mensagens de depuração 

   WiFi.mode(WIFI_STA);   -   # Configura o modo wi-fi para STA para conectar a uma rede existente

   # Configura o modo wi-fi para STA para conectar a uma rede existente 

  WiFi.begin(ssid, password);

  # inicia o processo de conexão


    while (WiFi.waitForConnectResult() != WL_CONNECT_FAILED){
    Serial.println("Conexão falhou! Rebooting... ");
    WiFi.begin(ssid, password);
    delay(5000);
}

# Aqui o codigo verifica se a conexão foi bem sucessida, caso contrario exibe uma mensagem de WL CONECCT FAILED 

ArduinoOTA.onStart()

# Função chamada quando uma atualização OTA é iniciada. Aqui, ele apenas imprime "Start" no monitor serial.

ArduinoOTA.onEnd()
# Quando a atualização OTA termina, ele imprime "End".

ArduinoOTA.onProgress()
# Esta função é chamada durante a atualização para mostrar o progresso. Ele calcula o progresso percentual e imprime no serial.

ArduinoOTA.begin()
#  Inicializa a funcionalidade de OTA. Depois disso, o dispositivo está pronto para receber atualizações.


void loop () {
  ArduinoOTA.handle();
}
# O loop principal do código chama ArduinoOTA.handle(), que verifica se a uma atualização OTA pendente e lida com ela.

