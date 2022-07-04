/*
Nutzen Sie diese Vorlage für die dritte Aufgabe. 
Folgende Dokumentation wird benötigt:
https://www.arduino.cc/reference/de/language/functions/time/millis/


Erweitern Sie die in Aufgabe 2 definierte union um den Datentyp unsigned long zur Speicherung des Zeitstempels
Das Union muss als Array mit zwei Eintragen deklariert werden. Der erste Eintrag ist für den Zeitstempel, der zweite für den Spannungswert

Die UUID für die Characteristic lautet: "4a980baa-1cc4-e7c1-c757-f1267dd021e8"
Die Webapp erkennt anhand der geänderten UUID, dass diese einen Zeitstempel enthält und verwendet diesen für die Darstellung. 
*/
#include <Arduino.h>
#include <ArduinoBLE.h>
#include <Chrono.h>
#include <stdio.h>
#include <string.h>

#define BUFFER 8

// =============================================================
// Hilfsvariablen
// =============================================================

typedef struct __attribute__( ( packed ) ){
  unsigned long time_stamp;
  float uin;
}myData_t;

typedef union 
{
  myData_t values;
  u_int8_t bytes[BUFFER];
}myData_tu;

myData_tu myData;

unsigned long delay_tmp = 5;

float val = 0.0;
// Erstellen Sie eine Variable die den analogen Eingangspin des Arduinos definiert
pin_size_t analogInputPin = A1;

// Die Webapp wartet auf einen BLE Service mit der UUID:
// "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
// Erstellen Sie das BLE Service Objekt mit der entsprechenden UUID
BLEService HeartRateService("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// in dieser erwartet sie eine Characteristic mit Spannungswerten (in micro Volt) mit der UUID:
// "4fafc201-1cc4-e7c1-c757-f1267dd021e8"
// Erstellen Sie das BLE Chracteristic Objekt mit der entsprechenden UUID, verwenden Sie den Typ BLEFloatCharacteristic
BLECharacteristic timeVoltCharacteristic("4a980baa-1cc4-e7c1-c757-f1267dd021e8", BLERead | BLENotify, BUFFER, true);
//BLEFloatCharacteristic voltageCharacteristic("4fafc201-1cc4-e7c1-c757-f1267dd021e8", BLERead | BLENotify);
// Fügen Sie die Characteristic Eigenschaften Read und Notify hinzu
// Uhr für die Signalabtastung
// Erstellen Sie ein Chrono Objekt für die regelmäßige Signalabtastung
Chrono chrono;



void setup() {
  // put your setup code here, to run once:

chrono.start(0);

// ini Data
myData.values.time_stamp = 0;
myData.values.uin = 0;

Serial.begin(115200);
//while (!Serial);

analogReadResolution(12);
// Initialisieren Sie die BLE Funktionalität, Geben Sie eine Fehlermeldung über die serielle Schnittstelle, 
// falls die Initialisierung nicht korrekt erfolgt.
if (!BLE.begin()){
  Serial.println("BLE initialization failed");
  while(1);
}
Serial.println("BLE initialized");
// Stellen Sie den BLE Gerätename "ECGprojectIBMT"
BLE.setDeviceName("ECGprojectIBMT");
// Setzen Sie oben definierten Service als Advertised Service
BLE.setAdvertisedService(HeartRateService);
// Fügen Sie dem Service die oben definierte Characteristic hinzu
HeartRateService.addCharacteristic(timeVoltCharacteristic);
//HeartRateService.addCharacteristic(timeStampCharacteristic);
//HeartRateService.addCharacteristic(voltageCharacteristic);
// Fügen sie den definierten Service hinzu
BLE.addService(HeartRateService);
// Setzen Sie dem Service einen initalen Wert 0
timeVoltCharacteristic.writeValue(myData.bytes, sizeof myData.bytes);
//timeStampCharacteristic.writeValue(0);
//voltageCharacteristic.writeValue(0);
// Starten Sie das Advertisement
BLE.advertise();

Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // put your main code here, to run repeatedly:

BLEDevice central = BLE.central();
if (central){
  Serial.println("connected to device");  
}
else {
  if (chrono.hasPassed(1000)){
    chrono.restart();
    Serial.println("no device found");
  }
}
  // Fragen Sie alle 5 ms den ADC ab und Schreiben sie den Spannungswert am Eingang des AFE in die Characteristic

while (central.connected()) {
  if (chrono.hasPassed(delay_tmp)) {

    chrono.restart();

    
    val = analogRead(analogInputPin);

    myData.values.time_stamp = millis();
    myData.values.uin =  ((((val/4096)*3.300)-(3.300/2))/1100)*1000;

    Serial.print( myData.values.time_stamp);
    Serial.print(" : ");
    Serial.println(myData.values.uin);
    timeVoltCharacteristic.writeValue(myData.bytes, sizeof myData.bytes);

    }
}
}
