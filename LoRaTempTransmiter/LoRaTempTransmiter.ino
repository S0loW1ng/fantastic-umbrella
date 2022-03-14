#include "helteck.h" // WLAN Lib
#define BAND 915E6; //Parameter definitions
String rssi = "RSSI --";
String packSize = "--";
String packet ;

#include <DHT.h> // Temp Sensor Lib
#define DHTPIN 2 // Parameter Deffinitions
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

//Global variables
float temperature = 99.99;


void setup() {
  Serial.begin(9600)
  
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  delay(2000);

  Heltec.display->display();
  delay(1000);
  Heltec.display->clear();
  
  
  
}

void loop() {
  getReadings();
  displayReadings()
  sendPacket("Temperature:" + String(temperature) + " C");
  delay(100)

}

void getReadings(){
  
  temperature = dht.readTemperature();
  Serial.printf("Temperature: %f \n",temperature);
  delay(50);
  
}

void displayReadings(){
  
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(0, 0, "Temperature: ");
  Heltec.display->drawString(90, 0, (String(temperature) + " C"));
  Heltec.display->display();
  delay(50);
  
}

void sendPacket(String str){

  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST); //RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
  LoRa.print(str);
  LoRa.endPacket();
  delay(50);
  
}
