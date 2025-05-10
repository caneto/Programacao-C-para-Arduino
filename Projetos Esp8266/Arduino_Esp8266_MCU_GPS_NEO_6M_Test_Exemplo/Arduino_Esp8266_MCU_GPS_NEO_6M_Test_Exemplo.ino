#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Defina os pinos RX e TX para o Software Serial 2
#define RX 14
#define TX 12

#define GPS_BAUD 9600

// O objeto TinyGPS++
TinyGPSPlus gps;

// Criar uma instância do Software Serial
SoftwareSerial gpsSerial(RX, TX);

void setup() {
  // Serial Monitor
  Serial.begin(115200);
  
  // Inicie a Serial 2 com os pinos RX e TX definidos e uma taxa de transmissão de 9600
  gpsSerial.begin(GPS_BAUD);
  Serial.println("Software Serial started at 9600 baud rate");
}

void loop() {
  // Esse esboço exibe informações sempre que uma nova frase é codificada corretamente.
  unsigned long start = millis();

  while (millis() - start < 1000) {
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
    if (gps.location.isUpdated()) {
      Serial.print("LAT: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("LONG: "); 
      Serial.println(gps.location.lng(), 6);
      Serial.print("SPEED (km/h) = "); 
      Serial.println(gps.speed.kmph()); 
      Serial.print("ALT (min)= "); 
      Serial.println(gps.altitude.meters());
      Serial.print("HDOP = "); 
      Serial.println(gps.hdop.value() / 100.0); 
      Serial.print("Satellites = "); 
      Serial.println(gps.satellites.value()); 
      Serial.print("Time in UTC: ");
      Serial.println(String(gps.date.year()) + "/" + String(gps.date.month()) + "/" + String(gps.date.day()) + "," + String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));
      Serial.println("");
    }
  }
}