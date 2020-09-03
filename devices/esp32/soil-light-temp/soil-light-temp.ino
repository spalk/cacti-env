#include "OTA.h"
#include <PCD8544.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_BME280.h>
#include <OneWire.h>
#include <WiFi.h>
#include <PubSubClient.h>
//#include "credentials.h"


unsigned long entry;

unsigned long timeing;
unsigned long timedelta = 60000; // send data to broker every 1 min

unsigned long timeingloop;
unsigned long timeloopdelta = 1000; // send data to broker every 1 min


//// WiFi & MQTT
WiFiClient espClient;
PubSubClient client(espClient);


//// Nokia display ////
/* Pinout:  SCK  18
            MOSI 23
            DC   19
            RST  14
            CS   5   */
// Backlight pin
#define BL 4
static PCD8544 lcd;
static const byte glyph[] = { 0x06, 0x09, 0x09, 0x06, 0x00 };  // degree sign
const int buttonPin = 35; // sensor button
bool backlight;

//// Light sensors ////
BH1750 bh1750_a;
BH1750 bh1750_b;
int light_level_a = 0;
int light_level_b = 0;
char light_level_a_str[6];
char light_level_b_str[6];



//// Soil Moisture Sensors ////
// Sensor 1
const int SMSensorPin1 = 32;
const int AirValue1 = 2550;   //max value   2531
const int WaterValue1 = 2100;  //min value    2100
int soil_moisture_level_a = 0;
char soil_moisture_level_a_str[6];

// Sensor 2
const int SMSensorPin2 = 34;
const int AirValue2 = 2400;   //max value  2481
const int WaterValue2 = 1850;  //min value  1900
int soil_moisture_level_b = 0;
char soil_moisture_level_b_str[6];

//// BME280 ////
Adafruit_BME280 bme; // I2C
float bme_t = 0;
int bme_p = 0;
int bme_h = 0;
char bme_t_str[6];
char bme_p_str[6];
char bme_h_str[6];


//// DS18B20 ////
OneWire ds(25);
float DS18B20_t = 0;
char DS18B20_t_str[6];


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  return;
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client", mqtt_login, mqtt_pass)) {
      Serial.println("connected");
      // Subscribe
      //client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}



void setup() {
    Serial.begin(115200);

    Serial.println("Booting");
    setupOTA("ESP32_Cacti-env");


    // wifi
    //setup_wifi();
    client.setServer(mqtt_server, 1883);
    //client.setCallback(callback);



    // PCD8544-compatible display resolution...
    lcd.begin(84, 48);

    // Set backlight (LOW - ON, HIGH - OFF)
    pinMode(BL, OUTPUT);
    digitalWrite(BL, HIGH);
    backlight = false;
    pinMode(buttonPin, INPUT);

    // Add the degree sigh to position "0" of the ASCII table...
    lcd.createChar(0, glyph);

    // Initialize the I2C bus and light sensors init
    Wire.begin(21, 22);
    bh1750_a.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire); //addr to gnd
    bh1750_b.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C, &Wire); //addr to 3v3

    // bme280 init
    bool bme280_status;
    bme280_status = bme.begin(0x76);
    if (!bme280_status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

}






// Get temperature from BME sensor
float getBmeT(){
    float result;
    result = bme.readTemperature();
    Serial.print("BME Temperature = ");
    Serial.print(result);
    Serial.println(" *C");
    TelnetStream.println("BME280 Temperature = " + String(result));
    return result;
}

// Get pressure from BME sensor
int getBmeP(){
    float result;
    result = bme.readPressure() / 100.0F * 0.75;
    Serial.print("BME Pressure = ");
    Serial.print(result);
    Serial.println(" mmhg");
    TelnetStream.println("BME280 Pressure = " + String(result));
    return int(result);
}

// Get humidity from BME sensor
int getBmeH(){
    float result;
    result = bme.readHumidity();
    Serial.print("Humidity = ");
    Serial.print(result);
    Serial.println(" %");
    TelnetStream.println("BME280 Humidity = " + String(result));
    return int(result);
}

// Get light level form sensor 1
int getL1(){
    float result;
    result = bh1750_a.readLightLevel();
    Serial.print("Light Level 1 = ");
    Serial.println(result);
    TelnetStream.println("BH1750 Light level 1 = " + String(result));
    return int(result);
}

// Get light level form sensor 2
int getL2(){
    float result;
    result = bh1750_b.readLightLevel();
    Serial.print("Light Level 2 = ");
    Serial.println(result);
    TelnetStream.println("BH1750 Light level 2 = " + String(result));
    return int(result);
}

// Get soil moister level form sensor 1
int getSM1(){
    float result;
    result = analogRead(SMSensorPin1);
    Serial.print("Soil Moisture Level 1 = ");
    Serial.print(result);
    Serial.println(" V");
    TelnetStream.println("Soil Moisture level 1 = " + String(result) + "V");
    int result_per;
    result_per = map(result, AirValue1, WaterValue1, 0, 100);
    Serial.print("Soil Moisture Level 1 = ");
    Serial.print(result_per);
    Serial.println(" %");
    TelnetStream.println("Soil Moisture level 1 = " + String(result_per) + "%");
    return result_per;
}

// Get soil moister level form sensor 2
int getSM2(){
    float result;
    result = analogRead(SMSensorPin2);
    Serial.print("Soil Moisture Level 2 = ");
    Serial.print(result);
    Serial.println(" V");
    TelnetStream.println("Soil Moisture level 2 = " + String(result) + "V");
    int result_per;
    result_per = map(result, AirValue2, WaterValue2, 0, 100);
    Serial.print("Soil Moisture Level 2 = ");
    Serial.print(result_per);
    Serial.println(" %");
    TelnetStream.println("Soil Moisture level 2 = " + String(result_per) + "%");
    return result_per;
}

// Get temperature from DS18B20
float getDS18B20T(){
    byte i;
    byte present = 0;
    byte data[12];
    byte addr[8];
    float celsius;

    if ( !ds.search(addr)){
        ds.reset_search();
        delay(500);
        for(byte i = 0; i < 10; i++) addr[i] = 0;
        ds.search(addr);
        //return;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1); // start conversion, with parasite power on at the end
    delay(750);
    present = ds.reset();
    ds.select(addr);
    ds.write(0xBE); // Read Scratchpad

    for ( i = 0; i < 9; i++){
        data[i] = ds.read();
    }


    // Convert the data to actual temperature
    int16_t raw = (data[1] << 8) | data[0];
    byte cfg = (data[4] & 0x60);


    if (cfg == 0x00) raw = raw & ~7; // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    celsius = (float)raw / 16.0;
    Serial.print("DS18B20 Temperature = ");
    Serial.print(celsius);
    Serial.println(" *C ");
    TelnetStream.println("DS18B20 Temperature = " + String(celsius));
    return celsius;
}

void loop() {
    entry=micros();

    ArduinoOTA.handle();

    if (millis() - timeingloop > timeloopdelta){

        // connection
        if (!client.connected()) {
            reconnect();
        }
        client.loop();

        // backlight button
        if (digitalRead(buttonPin)){
            if (backlight == true){
                digitalWrite(BL, HIGH);
                backlight = false;
            } else {
                digitalWrite(BL, LOW);
                backlight = true;
            }
        }

        // get data from sensors
        bme_t = getBmeT();
        bme_p = getBmeP();
        bme_h = getBmeH();
        light_level_a = getL1();
        light_level_b = getL2();
        soil_moisture_level_a = getSM1();
        soil_moisture_level_b = getSM2();
        DS18B20_t = getDS18B20T();

        // send data to broker
        if (millis() - timeing > timedelta){
            Serial.println("*** Sending data to broker ***");
            TelnetStream.println("*** Sending data to broker ***");
            dtostrf(bme_t, 5, 1, bme_t_str);
            dtostrf(bme_p, 3, 0, bme_p_str);
            dtostrf(bme_h, 3, 0, bme_h_str);
            dtostrf(DS18B20_t, 5, 1, DS18B20_t_str);
            dtostrf(light_level_a, 5, 1, light_level_a_str);
            dtostrf(light_level_b, 5, 1, light_level_b_str);

            // get avarage voltage from soil moister sensors
            int sml_avr_volt_a = 0;
            for (int i=0; i < 10; i++){
                sml_avr_volt_a = sml_avr_volt_a + analogRead(SMSensorPin1);
            }
            sml_avr_volt_a = sml_avr_volt_a/10;

            int sml_avr_volt_b = 0;
            for (int i=0; i < 10; i++){
                sml_avr_volt_b = sml_avr_volt_b + analogRead(SMSensorPin2);
            }
            sml_avr_volt_b = sml_avr_volt_b/10;

            dtostrf(sml_avr_volt_a, 4, 0, soil_moisture_level_a_str);
            dtostrf(sml_avr_volt_b, 4, 0, soil_moisture_level_b_str);

            timeing = millis();
            client.publish("izm/south-balcony/temperature/inside", bme_t_str);
            client.publish("izm/south-balcony/temperature/outside", DS18B20_t_str);
            client.publish("izm/south-balcony/pressure", bme_p_str);
            client.publish("izm/south-balcony/humidity", bme_h_str);
            client.publish("izm/south-balcony/light-intensity/sensor-01", light_level_a_str);
            client.publish("izm/south-balcony/light-intensity/sensor-02", light_level_b_str);
            client.publish("izm/south-balcony/soil-moisture/sensor-01", soil_moisture_level_a_str);
            client.publish("izm/south-balcony/soil-moisture/sensor-02", soil_moisture_level_b_str);
            TelnetStream.println("*** Sent OK!***");
        }

        //// SHOW CURRENT VALUES ON DISPLAY ////

        // Display Line 1
        lcd.setCursor(0, 0);
        lcd.print("T in =");
        lcd.print(bme_t, 1);
        lcd.print(" ");
        lcd.write(0); // degree sign
        lcd.print("C");

        // Display Line 2
        lcd.setCursor(0, 1);
        lcd.print("T out=");
        lcd.print(DS18B20_t, 1);
        lcd.print(" ");
        lcd.write(0); // degree sign
        lcd.print("C");

        // Display Line 3
        lcd.setCursor(0, 2);
        lcd.print("P=");
        lcd.print(bme_p);
        lcd.print("mm");

        lcd.setCursor(50, 2);
        lcd.print("H=");
        lcd.print(bme_h);
        lcd.print("%");

        // Display Line 4
        lcd.setCursor(0, 3);
        lcd.print("L1=");
        lcd.print(light_level_a);
        lcd.print(" units   ");

        // Display Line 5
        lcd.setCursor(0, 4);
        lcd.print("L2=");
        lcd.print(light_level_b);
        lcd.print(" units   ");

        // Display Line 6
        lcd.setCursor(0, 5);
        lcd.print("M1=");
        lcd.print(soil_moisture_level_a);
        lcd.print("% ");

        lcd.setCursor(42, 5);
        lcd.print("M2=");
        lcd.print(soil_moisture_level_b);
        lcd.print("% ");

        timeingloop = millis();
    }
}