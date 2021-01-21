/*
   Connection will same as we done in demo.ino program
*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"

//Include the library
#include <MQUnifiedsensor.h>

//Definitions for Gas Sensor
#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin A0 //Analog input 0 of your arduino
#define type "MQ-135" //MQ135
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO
#define RatioMQ135CleanAir 3.6//RS / R0 = 3.6 ppm  
//#define calibration_button 13 //Pin to calibrate your sensor

//Declare Sensor
MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);

#define DHTPIN 5     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

// LDR (Light Dependent Resistor)
#define ldr  35

// OLED Display
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define I2Cdisplay_SDA 33
#define I2Cdisplay_SCL 32
TwoWire I2Cdisplay = TwoWire(1);

float Acetona;
float NH4;
float Tolueno;
float CO2;
float Alcohol;
float CO;
float aqi;

//Declaring OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cdisplay, -1);

DHT dht(DHTPIN, DHTTYPE);

// Replace with your network credentials
const char* ssid     = "python";
const char* password = "python07";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "https://webserver0080.000webhostapp.com/data.php";

// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-data.php also needs to have the same key
String apiKeyValue = "azxcvbnml";

Adafruit_BMP280 bme;  // I2C

void setup() {
  Serial.begin(115200);

  //Set math model to calculate the PPM concentration and the value of constants for Gas Sensor
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135.init();
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for (int i = 1; i <= 10; i ++)
  {
    MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0 / 10);
  Serial.println("  done!.");

  //Seting Display
  I2Cdisplay.begin(I2Cdisplay_SDA, I2Cdisplay_SCL, 100000);
  // Initialize OLED Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);

  WiFi.begin(ssid, password);
  //  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3D for 128x64
  //  delay(200);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Connecting...");
  display.println("\n\n\n\n");
  display.println("       WARNING       ");
  display.println("     Turn ON WiFi    ");
  display.display();

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  display.clearDisplay();
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  display.setCursor(0, 0);
  display.println(" Connected to WiFi ");
  display.println("     IP Address     ");
  display.print("    ");
  display.println(WiFi.localIP());
  display.println("_____________________");
  display.println(" WebServer Connected ");
  display.println("         ...         ");
  display.println();
  display.print(" WiFi SSID -> ");
  display.println(ssid);
  display.display();
  delay(2000);
  display.clearDisplay();

  // (you can also pass in a Wire library object like &Wire2)
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring or change I2C address!");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println();
    display.println("       Warning       ");
    display.println(" Check BMP280 Wiring ");
    display.display();
    while (1);
  }

  Serial.println(F("DHTxx test!"));
  dht.begin();

  if (isinf(calcR0)) {
    Serial.println("Warning: Connection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply");
    while (1);
  }
  if (calcR0 == 0) {
    Serial.println("Warning: Connection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply");
    while (1);
  }
}

void loop() {


  //Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    delay(500);
    mq135_fun();
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float humi = dht.readHumidity();

    int ldrReading = map(analogRead(ldr), 0, 4095, 100, 0);

    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(bme.readTemperature())
                             + "&value2=" + String(bme.readPressure() / 100.0F) + "&value3=" + String(bme.readAltitude(1013.25)) + "&value4=" + String(humi) + "&value5=" + String(ldrReading) + "&value6=" + String(aqi) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);

    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");

    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
    dht11();
  }
  else {
    Serial.println("WiFi Disconnected");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(" WiFi Disconnected ! ");
    display.println("\n\n\n\n");
    display.println("       WARNING       ");
    display.println("     Turn ON WiFi    ");
    display.display();
  }
  //Send an HTTP POST request every 30 seconds
  delay(9000);
}

//void disp() {
//  float t = dht.readTemperature();
//  float f = dht.readTemperature(true);
//  float h = dht.readHumidity();
//  float hic = dht.computeHeatIndex(t, h, false);
//  display.clearDisplay();
//  display.setCursor(0, 0);
//  display.println("Live Weather Forcast");
//  display.println("_____________________");
//  display.println("DHT Temp : " + String(t) + " " + (char)247 + "C");
//  display.println("BMP Temp : " + String(bme.readTemperature()) + " " + (char)247 + "C");
//  display.println("Humi : " + String(h) + " %");
//  display.println("Heat In : " + String(hic) + " " + (char)247 + "C");
//  display.println("ATP : " + String(bme.readPressure() / 100.0F) + " hPa");
//  display.println("Alt : " + String(bme.readAltitude(1013.25)) + " m");
//  display.display();
//}

void dht11() {
  // Wait a few seconds between measurements.
  delay(500);
  //read gas sensor data...
  mq135_fun();
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  //  float hic = dht.computeHeatIndex(t, h, false);
  int ldrReading = map(analogRead(ldr), 0, 4095, 100, 0);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!!!"));
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println();
    display.println("Warning : ");
    display.println(" Check DHT11 Sensor ");
    display.display();
    return;
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Live Weather Forcast");
    display.println("_____________________");
    display.println("Temp : " + String(bme.readTemperature()) + " " + (char)247 + "C");
    display.println("AQI : " + String(aqi) + " PPM");
    display.println("Humi : " + String(h) + " %");
    display.println("ATP : " + String(bme.readPressure() / 100.0F) + " hPa");
    display.println("Alt : " + String(bme.readAltitude(1013.25)) + " m");
    display.println("Luminosity : " + String(ldrReading) + " %");
    display.display();
  }
}

void mq135_fun() {
  MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin

  MQ135.setA(605.18); MQ135.setB(-3.937); // Configurate the ecuation values to get CO concentration
  CO = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(77.255); MQ135.setB(-3.18); // Configurate the ecuation values to get Alcohol concentration
  Alcohol = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(110.47); MQ135.setB(-2.862); // Configurate the ecuation values to get CO2 concentration
  CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(44.947); MQ135.setB(-3.445); // Configurate the ecuation values to get Tolueno concentration
  Tolueno = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(102.2 ); MQ135.setB(-2.473); // Configurate the ecuation values to get NH4 concentration
  NH4 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  MQ135.setA(34.668); MQ135.setB(-3.369); // Configurate the ecuation values to get Acetona concentration
  Acetona = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup

  aqi = (CO + CO2 + Alcohol + Tolueno + NH4 + Acetona) / 6 ;
}
