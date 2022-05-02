#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_NeoPixel.h>

#define NUM_OF_LEDS 8
#define PIN 12
#define WIFILED 23
#define DHTTYPE DHT11
#define DHTPIN 4

const char *SSID = "You ssid";
const char *PWD = "You password";

// env variable
float temperature;
float humidity;
float internal;
// Wifi - check
unsigned long previousMillisWIFI = 0;
unsigned long intervalWIFI = 30000;

// DHT
DHT dht(DHTPIN, DHTTYPE);

// Web server running on port 80
WebServer server(80);

// Neopixel LEDs strip
Adafruit_NeoPixel pixels(NUM_OF_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[250];

void connectToWiFi();
void setupRouting();
void createJson(char *tag, float value, char *unit);
void addJsonObject(char *tag, float value, char *unit);
void readSensorData(void *parameter);
void getTemperature();
void getHumidity();
void getInternal();
void getEnv();
void handlePost();
void setupTask();
void log(String msg);

#ifdef __cplusplus
extern "C"
{
#endif
  uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

#include "ledBlinkModule.hpp"
#include "wifiModule.hpp"

void addJsonObject(char *tag, float value, char *unit)
{
  JsonObject obj = jsonDocument.createNestedObject();
  obj["type"] = tag;
  obj["value"] = value;
  obj["unit"] = unit;
}

void createJson(char *tag, float value, char *unit)
{
  jsonDocument.clear();
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}

void getTemperature()
{
  log("Info: Get temperature");
  createJson("temperature", temperature, "°C");
  server.send(200, "application/json", buffer);
}

void getHumidity()
{
  log("Info: Get humidity");
  createJson("humidity", humidity, "%");
  server.send(200, "application/json", buffer);
}

void getInternal()
{
  log("Info: Get internal");
  createJson("internal", internal, "mBar");
  server.send(200, "application/json", buffer);
}

void getEnv()
{
  log("Info: Get env");
  jsonDocument.clear();
  addJsonObject("temperature", temperature, "°C");
  addJsonObject("humidity", humidity, "%");
  addJsonObject("internal", internal, "mBar");
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}

void setupRouting()
{
  server.on("/temperature", getTemperature);
  server.on("/humidity", getHumidity);
  server.on("/internal", getInternal);
  server.on("/env", getEnv);
  server.on("/led", HTTP_POST, handlePost);

  // start server
  server.begin();
}

void readSensorData(void *parameter)
{
  for (;;)
  {

    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    internal = ((temprature_sens_read() - 32) / 1.8);
    log("Info: Read sensor data");

    // delay the task
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}

void handlePost()
{
  if (server.hasArg("plain") == false)
  {
    // handle error here
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);

  // Get RGB components
  int red = jsonDocument["red"];
  int green = jsonDocument["green"];
  int blue = jsonDocument["blue"];

  Serial.print("Info: Red: ");
  Serial.println(red);
  Serial.print("Info: Green: ");
  Serial.println(green);
  Serial.print("Info: Blue: ");
  Serial.println(blue);

  pixels.fill(pixels.Color(red, green, blue));
  pixels.show();
  // Respond to the client
  server.send(200, "application/json", "{}");
}

void setupTask()
{
  xTaskCreate(
      readSensorData,
      "Read sensor data",
      1000,
      NULL,
      1,
      NULL);
}

void log(String msg)
{
  Serial.println(msg);
}

void setup()
{
  Serial.begin(115200);
  delay(100);

  pinMode(WIFILED, OUTPUT);
  digitalWrite(WIFILED, LOW);

  // Sensor setup
  dht.begin();
  // WiFi
  connectToWiFi();
  // Task
  setupTask();
  // Routers
  setupRouting();
  // Initialize Neopixel
  pixels.begin();
}

void loop()
{
  yield();
  wifiLoop();
  server.handleClient();
}