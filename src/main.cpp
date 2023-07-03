#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "secret.h"

#define LED_BUILTIN 2
#define OUTLET_PIN 14

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
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // deserialize json
  StaticJsonDocument<244> doc;
  deserializeJson(doc, payload, length);

  // read values
  short unsigned int idx = doc["idx"];
  if(idx == 9) {
    short unsigned int nvalue = doc["nvalue"];
    if(nvalue == 0) {
      Serial.println("LED OFF");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(OUTLET_PIN, HIGH);
    } else if(nvalue == 1) {
      Serial.println("LED ON");
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(OUTLET_PIN, LOW);
    }
  }
}

void requestState() {
  // publish state
  StaticJsonDocument<244> doc;
  doc["command"] = "getdeviceinfo";
  doc["idx"] = 9;
  char buffer[244];
  serializeJson(doc, buffer);
  client.publish("domoticz/in", buffer);
  Serial.print("published: ");
  Serial.println(buffer);
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

      delay(1000);
      Serial.println("Requesting state... ");
      requestState();
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
  delay(1000);
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);

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
  Serial.print("Connecting to MQTT broker ");
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
}