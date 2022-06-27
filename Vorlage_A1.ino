/*
Diese Vorlage soll Ihnen helfen einen Einstieg in die Programmierung des Arduinos 
zu finden und Sie bei der Bewältigung der ersten Aufgabe unterstützen. Die notwendigen 
Befehle zur Umsetzung Ihrer Aufgabe finden Sie in der offiziellen Dokumantation von Arduino unter: 

//www.arduino.cc/reference/en/language/functions/communication/serial/
//www.arduino.cc/reference/en/language/functions/analog-io/analogread/
//www.arduino.cc/reference/en/language/functions/time/delay/
*/
double val = 0;
double uin1 = 0;
double uin2 = 0;
void setup() {
  // Dieser Teil Ihres Programms wird zu Beginn einmailg ausgeführt
// Initialisieren Sie eine serielle Kommunikation mit einer Baudrate von 115200
Serial.begin(115200);
// Ändern Sie die Auflösung des ADCs auf 12 Bit
analogReadResolution(12);
}

void loop() {
  // Dieser Teil wird in einer Schleife wiederholt ausgeführt

// Lesen Sie den Wert des ADC auf Pin A1 aus
val = analogRead(A1);
// Berechnen Sie den Wert der Eingangsspannung am ADC in V
uin1 = (((val/4096)*3.300)-(3.300/2))/1100;

// Berechnen Sie den Wert der differentiellen Eingansspanung am AFE in µV
uin2 = uin1*1000*1000;

// Senden Sie den Wert der differentiellen Eingansspanung über die serielle Schnittstelle
Serial.println(uin2);

// Pausieren Sie das Programm, so dass die Abtastrate des ADC 200 Hz beträgt (Vernachlässigen Sie den Einfluss der anderen Befehle)
delay(5);
}
