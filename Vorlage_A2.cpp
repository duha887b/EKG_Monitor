/*
Diese Vorlage soll Sie bei der Abarbeitung der Aufgabe 2 unterstützen. 
Zur Abarbeitung benötigen Sie Befehle die Sie in den folgenden Dokumentationen nachlesen können:
https://www.arduino.cc/en/Reference/ArduinoBLE
https://github.com/arduino-libraries/ArduinoBLE
https://github.com/SofaPirate/Chrono

Aufgabe 2 baut auf der vorherigen Aufgabe auf. Notwendige Schritte aus Aufgabe 1 werden nicht nochmals beschrieben. 
*/

// Biblotheken
// =============================================================
// Binden Sie die benötigten Bibliotheken "ArduinoBLE" und "Chrono" ein
#include <ArduinoBLE.h>
#include <Chrono.h>

// =============================================================
// Hilfsvariablen
// =============================================================
float uin = 0;
float val = 0;
unsigned int delay_tmp = 5;



// Erstellen Sie eine Variable die den analogen Eingangspin des Arduinos definiert
pin_size_t analogInputPin = A1;

// Die Webapp wartet auf einen BLE Service mit der UUID:
// "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
// Erstellen Sie das BLE Service Objekt mit der entsprechenden UUID
BLEService HeartRateService("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// in dieser erwartet sie eine Characteristic mit Spannungswerten (in micro Volt) mit der UUID:
// "4fafc201-1cc4-e7c1-c757-f1267dd021e8"
// Erstellen Sie das BLE Chracteristic Objekt mit der entsprechenden UUID, verwenden Sie den Typ BLEFloatCharacteristic
BLEFloatCharacteristic voltageCharacteristic("4fafc201-1cc4-e7c1-c757-f1267dd021e8", BLERead | BLENotify);
// Fügen Sie die Characteristic Eigenschaften Read und Notify hinzu
//voltageCharacteristic.properties(BLERead | BLENotify);
// Uhr für die Signalabtastung
// Erstellen Sie ein Chrono Objekt für die regelmäßige Signalabtastung
Chrono chrono;
void setup() {
  // put your setup code here, to run once:

// Initalisieren Sie die serielle Kommunikation und den ADC wie in Aufgabe 1
Serial.begin(115200);
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
//BLE.setAdvertisedServiceUuid("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// Fügen Sie dem Service die oben definierte Characteristic hinzu
HeartRateService.addCharacteristic(voltageCharacteristic);
// Fügen sie den definierten Service hinzu
BLE.addService(HeartRateService);
// Setzen Sie dem Service einen initalen Wert 0
voltageCharacteristic.writeValue(uin);
// Starten Sie das Advertisement
BLE.advertise();
Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // put your main code here, to run repeatedly:
// prüfen Sie ob eine Verbindung zu einen BLE central besteht, wenn ja:
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
    uin = ((((val/4096)*3.300)-(3.300/2))/1100)*1000*1000;//microvolt
    Serial.println(uin);
    voltageCharacteristic.writeValue(uin);
  }
}

  

// Nutzen Sie für die zyklische Abfrage die Chrono Bibliothek
  
}
