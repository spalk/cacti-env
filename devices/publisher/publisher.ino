#include "OTA.h"
//#include "credentials.h"
#include <PubSubClient.h>
//#include <ESP8266WiFi.h>
#include <Adafruit_BME280.h>

// WiFi & MQTT
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];

// delays
unsigned long timing;
unsigned long timedelta = 60000;

// BME280
Adafruit_BME280 bme; // I2C
float h, t, p;
float bme_t = 0;
int bme_p = 0;
int bme_h = 0;
char bme_t_str[6];
char bme_p_str[6];
char bme_h_str[6];

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266BedroomCond", mqtt_login, mqtt_pass)) {
      Serial.println("connected");
      // Subscribe
      //client.subscribe("izm/south-balcony/temperature/outside");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

float getBmeT(){
    t = 0;
    float valt;
    for (int i=0; i<30; i++) {
        delay (20);
        valt = bme.readTemperature();
        t = t+valt;
    }
    t=t/30;
    return t-1;
}

float getBmeP(){
    p = 0;
    float valp;
    for (int i=0; i<30; i++) {
        delay (20);
        valp = bme.readPressure();
        p = p+valp;
    }
    p=p/30/100.0F*0.75;  //mmhg
    return p;
}

float getBmeH() {
    h = 0;
    float valh;
    for (int i=0; i<30; i++) {
        delay (20);
        valh = bme.readHumidity();
        h = h+valh;
    }
    h=h/30;
    return h;
}


void setup() {
    Serial.begin(115200);
    Serial.println("Booting");

    setupOTA("Wemos_Bedroom_Cond");

    client.setServer(mqtt_server, 1883);

    if (!bme.begin()) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

    timing = millis();
}


void loop() {
    ArduinoOTA.handle();
    delay(20);

    // connection
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // get data from sensors
    bme_t = getBmeT();
    bme_p = getBmeP();
    bme_h = getBmeH();

    if (millis() - timing > timedelta){
        dtostrf(bme_t, 5, 1, bme_t_str);
        dtostrf(bme_p, 3, 0, bme_p_str);
        dtostrf(bme_h, 3, 0, bme_h_str);

        Serial.println("Sending data to broker ");

        client.publish("izm/bedroom/temperature", bme_t_str);
        client.publish("izm/bedroom/pressure", bme_p_str);
        client.publish("izm/bedroom/humidity", bme_h_str);

        timing = millis();
    }

}
