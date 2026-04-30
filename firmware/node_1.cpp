#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// --- Configuration: ONLY DIFFERENCE IS node_id ---
const char* node_id = "TX_01"; 
const char* ssid = "Timothy's iPhone";
const char* password = "ameeltoocute";
const char* mqtt_server = "172.20.10.3";

// --- MQTT Setup ---
WiFiClient espClient;
PubSubClient client(espClient);
const char* telemetry_topic = "fwncvsd/telemetry/live";
const char* status_topic = "fwncvsd/status";

unsigned long lastMsg = 0;
const int PUBLISH_INTERVAL = 500; 

void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to WiFi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected. IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection for ");
    Serial.print(node_id);
    Serial.print("...");
    
    String clientId = "ESP32_Node_";
    clientId += node_id;
    
    // LWT: If this node drops, broker publishes "offline" to status topic
    String lwtMessage = "{\"node\":\"" + String(node_id) + "\", \"status\":\"offline\"}";
    
    if (client.connect(clientId.c_str(), status_topic, 1, true, lwtMessage.c_str())) {
      Serial.println("connected");
      // Publish "online" status immediately upon connection
      String onlineMsg = "{\"node\":\"" + String(node_id) + "\", \"status\":\"online\"}";
      client.publish(status_topic, onlineMsg.c_str(), true);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); 
  setup_wifi();
  client.setKeepAlive(2);
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); 

  unsigned long now = millis();
  if (now - lastMsg > PUBLISH_INTERVAL) {
    lastMsg = now;

    // Simulate voltage fluctuating around 120V
    float simulated_voltage = 120.0 + ((random(-150, 150) / 100.0)); 

    StaticJsonDocument<200> doc;
    doc["node"] = node_id;
    doc["voltage"] = serialized(String(simulated_voltage, 2));

    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);

    Serial.print("Publishing ");
    Serial.print(node_id);
    Serial.print(": ");
    Serial.println(jsonBuffer);
    client.publish(telemetry_topic, jsonBuffer);
  }
}
