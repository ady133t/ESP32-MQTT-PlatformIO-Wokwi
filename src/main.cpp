
#include "DHT.h"
#include <WiFi.h>
#include <PubSubClient.h>


const int DHT22PIN = 12;
#define DHTTYPE DHT22
String machineID = "ESP32-01";
const char* mqtt_server = "broker.emqx.io";

DHT dht(DHT22PIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin (115200);
  Serial.begin(115200);

   WiFi.begin("Wokwi-GUEST", "");
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  client.setServer(mqtt_server, 1883);
  

  dht.begin();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "device1";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      // Serial.println("connected");
      // // Once connected, publish an announcement...
      // client.publish("outTopic", "hello world");
      // // ... and resubscribe
      // client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

 char* convertStringtoCharArray(String str)
 {

  // Calculate length and allocate a buffer
    int bufferSize = str.length() + 1;  // +1 for the null terminator
    char* charArray = new char[bufferSize];

    // Copy the string to the char array
    str.toCharArray(charArray, bufferSize);

    return charArray;

 }


char* topic = convertStringtoCharArray(machineID + "_mqtt");
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Humidity: ");
  Serial.println(humi);

  //String machineID = "device1";
  String mqttData = "{Id:\"" +machineID+ "\",value:{temperature:" + temp + ",humidity:" + humi + "}}" ;
  
  client.publish(topic, convertStringtoCharArray(mqttData));

  delay (500);

}



