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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // deserialize json
  StaticJsonDocument<244> doc;
  deserializeJson(doc, payload, length);

  // read values
  short unsigned int idx = doc["idx"];
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("JK_ESP8266_0", mqtt_username, mqtt_pwd)) {
      Serial.print("connected with ");
      Serial.print(mqtt_username);
      Serial.println("!");
      // Subscribe
      Serial.println("Subscribing to topics:");
      Serial.println("esp/out/9");
      client.subscribe("esp/out/9");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds...");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


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
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}