#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// O objeto TinyGPS++
TinyGPSPlus gps;

// A conexão serial com o dispositivo GPS
SoftwareSerial ss(RXPin, TXPin);

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop(){
  // Esse sketch exibe informações sempre que uma nova sentença é codificada corretamente.
  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      // Latitude em graus (double)
      Serial.print("Latitude= "); 
      Serial.print(gps.location.lat(), 6);      
      // Longitude em graus (double)
      Serial.print(" Longitude= "); 
      Serial.println(gps.location.lng(), 6); 
       
      // Latitude bruta em graus inteiros
      Serial.print("Raw latitude = "); 
      Serial.print(gps.location.rawLat().negative ? "-" : "+");
      Serial.println(gps.location.rawLat().deg); 
      // ... e bilionésimos (u16/u32)
      Serial.println(gps.location.rawLat().billionths);
      
      // Longitude bruta em graus inteiros
      Serial.print("Raw longitude = "); 
      Serial.print(gps.location.rawLng().negative ? "-" : "+");
      Serial.println(gps.location.rawLng().deg); 
      // ... e bilionésimos (u16/u32)
      Serial.println(gps.location.rawLng().billionths);

      // Data bruta no formato DDMMYY (u32)
      Serial.print("Raw date DDMMYY = ");
      Serial.println(gps.date.value()); 

      // Ano (2000+) (u16)
      Serial.print("Year = "); 
      Serial.println(gps.date.year()); 
      // Mês (1-12) (u8)
      Serial.print("Month = "); 
      Serial.println(gps.date.month()); 
      // Dia (1-31) (u8)
      Serial.print("Day = "); 
      Serial.println(gps.date.day()); 

      // Hora bruta no formato HHMMSSCC (u32)
      Serial.print("Raw time in HHMMSSCC = "); 
      Serial.println(gps.time.value()); 

      // Hora (0-23) (u8)
      Serial.print("Hour = "); 
      Serial.println(gps.time.hour()); 
      // Minuto (0-59) (u8)
      Serial.print("Minute = "); 
      Serial.println(gps.time.minute()); 
      // Segundo (0-59) (u8)
      Serial.print("Second = "); 
      Serial.println(gps.time.second()); 
      // Segundo (0-59) (u8)
      Serial.print("Centisecond = "); 
      Serial.println(gps.time.centisecond()); 

      // Velocidade bruta em centésimos de nó (i32)
      Serial.print("Raw speed in 100ths/knot = ");
      Serial.println(gps.speed.value()); 
      // Velocidade em nós (double)
      Serial.print("Speed in knots/h = ");
      Serial.println(gps.speed.knots()); 
      // Velocidade em milhas por hora (double)
      Serial.print("Speed in miles/h = ");
      Serial.println(gps.speed.mph()); 
      // Velocidade em metros por segundo (double)
      Serial.print("Speed in m/s = ");
      Serial.println(gps.speed.mps()); 
      // Velocidade em quilômetros por hora (double)
      Serial.print("Speed in km/h = "); 
      Serial.println(gps.speed.kmph()); 

      // Curso bruto em centésimos de grau (i32)
      Serial.print("Raw course in degrees = "); 
      Serial.println(gps.course.value()); 
      // Curso em graus (double)
      Serial.print("Course in degrees = "); 
      Serial.println(gps.course.deg()); 

      // Altitude bruta em centímetros (i32)
      Serial.print("Raw altitude in centimeters = "); 
      Serial.println(gps.altitude.value()); 
      // Altitude em metros (double)
      Serial.print("Altitude in meters = "); 
      Serial.println(gps.altitude.meters()); 
      // Altitude em milhas (double)
      Serial.print("Altitude in miles = "); 
      Serial.println(gps.altitude.miles()); 
      // Altitude em quilômetros (double)
      Serial.print("Altitude in kilometers = "); 
      Serial.println(gps.altitude.kilometers()); 
      // Altitude em pés (double)
      Serial.print("Altitude in feet = "); 
      Serial.println(gps.altitude.feet()); 

      // Número de satélites em uso (u32)
      Serial.print("Number os satellites in use = "); 
      Serial.println(gps.satellites.value()); 

      // Dimensão horizontal da precisão (100ths-i32)
      Serial.print("HDOP = "); 
      Serial.println(gps.hdop.value()); 
    }
  }
}