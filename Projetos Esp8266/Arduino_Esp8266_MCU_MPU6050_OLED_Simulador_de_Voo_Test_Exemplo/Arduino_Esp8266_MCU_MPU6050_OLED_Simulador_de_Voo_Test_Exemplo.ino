#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MPU 0x68

int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float angleX = 0, angleY = 0;
float accAngleX, accAngleY;
float gyroXrate, gyroYrate;
unsigned long timer;
float dt;

float altitude = 1200;   // Aktuelle Höhe
float speed = 250;       // Feste Geschwindigkeit
float heading = 90;      // Richtung (0° = Nord)

void showLoadingScreen() {
  for (int i = 0; i <= 100; i += 5) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20, 0);
    display.print("CapSistema");
    display.setCursor(10, 56);
    display.print("www.capsistema.com.br");
    display.setCursor(30, 20);
    display.print("Loading");
    for (int d = 0; d < (i / 33) + 1; d++) display.print(".");
    display.drawRect(14, 34, 100, 10, WHITE);
    display.fillRect(14, 34, i, 10, WHITE);
    display.display();
    delay(100);
  }
  delay(300);
}

void setup() {
  Wire.begin(12, 14);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.clearDisplay();
  showLoadingScreen();

  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  timer = millis();
}

void readMPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}

// --- ALT / SPD Info ---
void drawAltSpeed() {
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("ALT:");
  display.print((int)altitude);
  display.print("m");

  display.setCursor(64, 0);
  display.print("SPD:");
  display.print((int)speed);
  display.print("k");
}

// --- Kompass ---
void drawCompass(float heading) {
  int baseX = SCREEN_WIDTH / 2;
  int baseY = 61;
  int tickSpacing = 10;

  for (int i = -6; i <= 6; i++) {
    int pos = baseX + i * tickSpacing - (int)(heading / 5) % tickSpacing;
    if (pos >= 0 && pos <= SCREEN_WIDTH) {
      if ((int)((heading / 5) + i) % 18 == 0) {
        display.drawLine(pos, baseY - 6, pos, baseY, WHITE);
        display.setCursor(pos - 3, baseY - 12);
        display.print("N");
      } else if ((int)((heading / 5) + i) % 18 == 4) {
        display.setCursor(pos - 3, baseY - 12);
        display.print("E");
      } else if ((int)((heading / 5) + i) % 18 == 9) {
        display.setCursor(pos - 3, baseY - 12);
        display.print("S");
      } else if ((int)((heading / 5) + i) % 18 == 13) {
        display.setCursor(pos - 3, baseY - 12);
        display.print("W");
      } else {
        display.drawLine(pos, baseY - 3, pos, baseY, WHITE);
      }
    }
  }

  display.drawTriangle(baseX - 3, baseY + 1, baseX + 3, baseY + 1, baseX, baseY - 4, WHITE);
}

// --- Pitch & Roll simuliert Höhe und Kurs ---
void updateFlightPhysics() {
  // Pitch (angleY) beeinflusst Höhe
  altitude += angleY * 0.1;
  if (altitude < 0) altitude = 0;

  // Roll (angleX) beeinflusst Heading
  heading += angleX * 0.05;
  if (heading >= 360) heading -= 360;
  if (heading < 0) heading += 360;
}

// --- Horizon ---
void drawHorizon(float roll, float pitch) {
  int centerX = SCREEN_WIDTH / 2;
  int centerY = 38;
  int horizonWidth = 200;

  float radRoll = roll * 0.01745;
  int offsetY = pitch * 0.6;

  int x1 = centerX - cos(radRoll) * horizonWidth;
  int y1 = centerY - sin(radRoll) * horizonWidth + offsetY;
  int x2 = centerX + cos(radRoll) * horizonWidth;
  int y2 = centerY + sin(radRoll) * horizonWidth + offsetY;

  display.drawLine(x1, y1, x2, y2, WHITE);

  // Flugzeug Nase
  display.drawLine(centerX - 8, centerY, centerX + 8, centerY, WHITE);
  display.drawTriangle(centerX - 4, centerY + 6, centerX + 4, centerY + 6, centerX, centerY + 12, WHITE);
}

// --- HUD Rahmen ---
void drawFrame() {
  display.drawRect(0, 12, 128, 50, WHITE);
}

void loop() {
  readMPU();

  unsigned long now = millis();
  dt = (now - timer) / 1000.0;
  timer = now;

  gyroXrate = GyX / 131.0;
  gyroYrate = GyY / 131.0;

  angleX += gyroXrate * dt;
  angleY += gyroYrate * dt;

  accAngleX = atan2(AcY, AcZ) * 180 / 3.141592;
  accAngleY = atan2(-AcX, AcZ) * 180 / 3.141592;

  angleX = 0.96 * angleX + 0.04 * accAngleX;
  angleY = 0.96 * angleY + 0.04 * accAngleY;

  // Simuliertes Flugverhalten
  updateFlightPhysics();

  display.clearDisplay();
  drawAltSpeed();
  drawFrame();
  drawHorizon(angleX, angleY);
  drawCompass(heading);
  display.display();
  delay(20);
}