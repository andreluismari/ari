#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

// Credenciais Wi-Fi
#define WIFI_SSID "andre"
#define WIFI_PASS "andre1997"

// Credenciais SinricPro
#define APP_KEY "870654aa-d09a-4b1b-b1f9-843ae434f0de"
#define APP_SECRET "37da78b2-e1dc-4150-8f29-a4631138d481-bc6dc792-d304-43fe-806b-eb21d3d66c59"
#define SWITCH_ID "6733d5c5e68a1fbf3fa9cb62"
#define BAUD_RATE 9600

// Pinos GPIO
#define BUTTON_PIN 0 // GPIO para o BOTÃO (inverso: LOW = pressionado, HIGH = liberado)
#define RELE_PIN 5

bool myPowerState = false; // Estado atual do dispositivo (ligado/desligado)
unsigned long lastBtnPress = 0; // Última vez que o botão foi pressionado (debounce)

// Callback para o SinricPro
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("Dispositivo %s foi %s (via SinricPro) \r\n", deviceId.c_str(), state ? "ligado" : "desligado");
  myPowerState = state;
  digitalWrite(RELE_PIN, myPowerState ? LOW : HIGH);
  return true;
}

// Trata o pressionamento do botão físico
void handleButtonPress() {
  unsigned long actualMillis = millis();
  if (digitalRead(BUTTON_PIN) == LOW && actualMillis - lastBtnPress > 1000) {
    myPowerState = !myPowerState;
    digitalWrite(RELE_PIN, myPowerState ? LOW : HIGH);
    // Envia o novo estado para o SinricPro
    SinricProSwitch &mySwitch = SinricPro[SWITCH_ID];
    mySwitch.sendPowerStateEvent(myPowerState);
    Serial.printf("Dispositivo %s foi %s (manualmente via botão)\r\n",
                  mySwitch.getDeviceId().c_str(), myPowerState ? "ligado" : "desligado");
    lastBtnPress = actualMillis;
  }
}

// Conecta ao Wi-Fi
void setupWiFi() {
  Serial.printf("\r\n[WiFi]: Conectando");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("conectado!\r\n[WiFi]: Endereço IP é %s\r\n", WiFi.localIP().toString().c_str());
}

// Configura a conexão com o SinricPro
void setupSinricPro() {
  SinricProSwitch &mySwitch = SinricPro[SWITCH_ID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.onConnected([]() { Serial.printf("Conectado ao SinricPro\r\n"); });
  SinricPro.onDisconnected([]() { Serial.printf("Desconectado do SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// Função de configuração inicial
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, HIGH);
  Serial.begin(BAUD_RATE);
  setupWiFi();
  setupSinricPro();
}

// Loop principal
void loop() {
  handleButtonPress();
  SinricPro.handle();
}
