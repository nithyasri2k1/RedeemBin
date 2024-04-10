#include <HardwareSerial.h>
#include <PubSubClient.h>
#include <WiFi.h>

HardwareSerial ScaleSerial(2); // Use Serial2 for communication with Arduino Uno

// Update these with your Wi-Fi credentials and MQTT broker details
const char* ssid = "project";
const char* password = "project@123";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883; // Default MQTT port
const char* mqtt_username = "project";
const char* mqtt_password = "project@123";
const char* mqtt_topic = "load_cell";

WiFiClient espClient;
PubSubClient client(espClient);

String receivedData = ""; // To store the received data as a string

void setup() {
  Serial.begin(115200);
  ScaleSerial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set MQTT broker and credentials
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  connectToMQTT();
}

void connectToMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
     if (client.connect("Arduino", mqtt_username, mqtt_password)){
      Serial.println("Connected to MQTT");
      client.subscribe(mqtt_topic); // Subscribe to the same topic for two-way communication
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages here if needed
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }

  if (ScaleSerial.available()) {
    char c = ScaleSerial.read();
    Serial.write(c); // Forward data to Serial Monitor on ESP32

    // Collect received data into a string
    receivedData += c;

    // Check if you've received a complete message (e.g., terminated with '\n')
    if (c == '\n') {
      // Publish the received data as a string to the MQTT topic
      client.publish(mqtt_topic, receivedData.c_str());
      receivedData = ""; // Clear the received data
      delay(3000);
    }
    
  }

  client.loop();
}
