#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "Gabriel";
const char* password = "1234567a";

void setup () {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECT_FAILED){
    Serial.println("Conexão falhou! Rebooting... ");
    WiFi.begin(ssid, password);
    delay(5000);
}

ArduinoOTA.onStart([](){
  Serial.println("Start");
});
ArduinoOTA.onEnd([](){
  Serial.println("\nEnd");
});
ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
  Serial.printf("progress: %u%%\r", (progress / (total / 100)));
});
ArduinoOTA.onError([](ota_error_t error) {
  Serial.printf("Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
});
ArduinoOTA.begin();
Serial.println("pronto");
Serial.print("IP: ");
Serial.println(WiFi.localIP());
}

void loop () {
  ArduinoOTA.handle();
}