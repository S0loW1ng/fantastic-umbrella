#include "ThingSpeak.h"
#include "secrets.h"

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

// Set up temperature sensor
#define TEMP_SENSE_PIN D2
OneWire oneWire(TEMP_SENSE_PIN);
DallasTemperature temperatureSensor(&oneWire);
float temperatureC;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  while (!Serial) {}

  temperatureSensor.setWaitForConversion(true);
  temperatureSensor.begin();

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);
}

void loop() {

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  
  // Get the current temperature
  temperatureSensor.requestTemperaturesByIndex(0);
  temperatureC = temperatureSensor.getTempCByIndex(0);

  // Write value to Field 1 of a ThingSpeak Channel
  int httpCode = ThingSpeak.writeField(myChannelNumber, 1, temperatureC, myWriteAPIKey);

  if (httpCode == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }

  // Wait 20 seconds to update the channel again
  Serial.println("Deep sleeping for 20 seconds.");
  ESP.deepSleep(20e6);
}