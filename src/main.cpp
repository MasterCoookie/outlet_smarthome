#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "secret.h"

// wifi connection definitions
WiFiClient espClient;
PubSubClient client(espClient);
// wifi connection definitions end

// mqtt definitions
const char* mqtt_server = "1.tcp.eu.ngrok.io";
short int mqtt_port = 21589;

void setup() {
  Serial.begin(115200);

  // wifi connection starts
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
  // wifi connection ends

  // mqtt connection starts
  // PubSubClient client(mqtt_server, 1883, callback, espClient);


}

void loop() {
  Serial.println("Dupa");
  delay(1000);
}