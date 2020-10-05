#include <ESP8266WiFi.h>
#include <DHT.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <Adafruit_Sensor.h>
#define DHTTYPE DHT11

#define dht_dpin D4
DHT dht(dht_dpin, DHTTYPE);

// wifi
const char* ssid = "KT_GiGA_2G_Wave2_2BA4";
const char* password = "dcdd9he125";
WiFiClient client;

// thingSpeak 
unsigned long ChannelID = 1136604;
const char* WriteAPIKey = "O5BUVKTNYCTDYAX8";

//DHT
float t; 
float h;
uint8_t temperature, humidity;
unsigned long lastCheck;


void setup() {
  Serial.begin(115200);
  delay(10);
    
  initWiFi();
  ThingSpeak.begin(client);

  lastCheck = 0;
}

void loop() {
  float c_time = 60000 * 0.1;
  if(millis() - lastCheck > c_time) {
    static boolean data_state = false;
    t = dht.readTemperature();
    h = dht.readHumidity();
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.println("C");
    Serial.print("Humidity : ");
    Serial.print(h);
    Serial.println("%");

    if(data_state){
      ThingSpeak.writeField(ChannelID, 1, t, WriteAPIKey);
      data_state = false;
    } else {
      ThingSpeak.writeField(ChannelID, 2, h, WriteAPIKey);
      data_state = true;
    }

    lastCheck = millis();
  }
 
  delay(100);
}

void initWiFi(){
  Serial.println();
  Serial.println();
  Serial.println("Connectiong to ssid ...");
  // attempt to connect to WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  // 접속성공!
  Serial.println();
  Serial.println("Connected WiFi");
  //사용하는 ip출력
  Serial.println(WiFi.localIP());
  Serial.println();
}
