#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MPU 0x68

// --- MPU Variablen ---
int16_t AcX, AcY, AcZ, GyX, GyY, GyZ;
float angleY = 0;
float angleX = 0;

// --- Spielvariablen ---
struct Obstacle {
  int x, y;
  int type;     // 0 = Asteroid, 1 = Enemy Ship
  int hitsLeft; // Trefferanzahl
  bool active;
  bool exploding;
  int explodeFrame;
};

#define MAX_OBSTACLES 5
Obstacle obstacles[MAX_OBSTACLES];

struct Bullet {
  int x, y;
  bool active;
};

#define MAX_BULLETS 5
Bullet bullets[MAX_BULLETS];

int shipX = 10;
int shipY = 30;
bool shipBlink = false;

int score = 0;
float distanceTravel = 0;
int speed = 2;
bool gameOver = false;
unsigned long lastBulletTime = 0;
unsigned long lastSpawnTime = 0;

// --- Loading Screen (nur Start) ---
void showLoadingScreen() {
  for (int i = 0; i <= 100; i += 10) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(15, 0);
    display.print("HD STARSHIP");
    display.setCursor(10, 56);
    display.print("www.hdrobotics.de");
    display.setCursor(30, 20);
    display.print("Loading");
    for (int d = 0; d < (i / 20) + 1; d++) display.print(".");
    display.drawRect(14, 34, 100, 10, WHITE);
    display.fillRect(14, 34, i, 10, WHITE);
    display.display();
    delay(150);
  }
  delay(500);
}

// --- MPU lesen ---
void readMPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);

  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();

  angleY = atan2(AcX, AcZ) * 180 / 3.14159;
  angleX = atan2(AcY, AcZ) * 180 / 3.14159;
}

// --- Hindernisse erzeugen ---
void spawnObstacle() {
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (!obstacles[i].active) {
      obstacles[i].active = true;
      obstacles[i].x = 120;
      obstacles[i].y = random(15, 55);
      obstacles[i].type = random(0, 2);
      obstacles[i].hitsLeft = (obstacles[i].type == 0) ? 1 : 2;
      obstacles[i].exploding = false;
      obstacles[i].explodeFrame = 0;
      break;
    }
  }
}

// --- Schuss abfeuern ---
void shoot() {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (!bullets[i].active) {
      bullets[i].active = true;
      bullets[i].x = shipX + 8;
      bullets[i].y = shipY + 2;
      break;
    }
  }
}

// --- Explosionseffekt ---
void startExplosion(Obstacle &obs) {
  obs.exploding = true;
  obs.explodeFrame = 0;
}

// --- Game Update ---
void updateGame() {
  shipY += (int)(angleY * 0.1);
  shipX += (int)(angleX * 0.05);

  if (shipY < 15) shipY = 15;
  if (shipY > 50) shipY = 50;
  if (shipX < 5) shipX = 5;
  if (shipX > 25) shipX = 25;

  for (int i = 0; i < MAX_BULLETS; i++) {
    if (bullets[i].active) {
      bullets[i].x += 4;
      if (bullets[i].x > 128) bullets[i].active = false;
    }
  }

  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacles[i].active && !obstacles[i].exploding) {
      obstacles[i].x -= speed;
      if (obstacles[i].x < 0) obstacles[i].active = false;

      if (obstacles[i].x < shipX + 8 && obstacles[i].x + 6 > shipX &&
          obstacles[i].y < shipY + 6 && obstacles[i].y + 6 > shipY) {
        gameOver = true;
      }

      for (int b = 0; b < MAX_BULLETS; b++) {
        if (bullets[b].active &&
            bullets[b].x > obstacles[i].x &&
            bullets[b].x < obstacles[i].x + 6 &&
            bullets[b].y > obstacles[i].y &&
            bullets[b].y < obstacles[i].y + 6) {
          bullets[b].active = false;
          obstacles[i].hitsLeft--;
          if (obstacles[i].hitsLeft <= 0) {
            startExplosion(obstacles[i]);
            score += 10;
          }
        }
      }
    }

    if (obstacles[i].exploding) {
      obstacles[i].explodeFrame++;
      if (obstacles[i].explodeFrame > 5) {
        obstacles[i].active = false;
        obstacles[i].exploding = false;
      }
    }
  }

  distanceTravel += 0.1;
  if ((int)distanceTravel % 100 == 0 && speed < 6) speed++;

  if (millis() - lastBulletTime > 500) {
    shoot();
    lastBulletTime = millis();
  }

  if (millis() - lastSpawnTime > 1200) {
    spawnObstacle();
    lastSpawnTime = millis();
  }
}

// --- Game Draw ---
void drawGame() {
  display.clearDisplay();

  // Punkte & Distanz
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("P:");
  display.print(score);
  display.setCursor(64, 0);
  display.print("D:");
  display.print((int)distanceTravel);

  // Spielfeldrahmen
  display.drawRect(0, 12, 128, 52, WHITE);

  // Raumschiff
  if (!shipBlink) {
    display.fillRect(shipX, shipY, 8, 4, WHITE);
    display.drawTriangle(shipX - 2, shipY + 2, shipX, shipY, shipX, shipY + 4, WHITE);
    // Flammen
    display.drawLine(shipX - 3, shipY + 1, shipX - 5, shipY + 2, WHITE);
    display.drawLine(shipX - 3, shipY + 3, shipX - 5, shipY + 2, WHITE);
  }

  // Bullets
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (bullets[i].active) {
      display.drawPixel(bullets[i].x, bullets[i].y, WHITE);
    }
  }

  // Hindernisse
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    if (obstacles[i].active) {
      if (obstacles[i].exploding) {
        if (obstacles[i].explodeFrame % 2 == 0) {
          display.drawCircle(obstacles[i].x, obstacles[i].y, 4, WHITE);
        }
      } else {
        if (obstacles[i].type == 0) {
          display.drawCircle(obstacles[i].x, obstacles[i].y, 3, WHITE);
        } else {
          display.drawRect(obstacles[i].x, obstacles[i].y, 6, 4, WHITE);
        }
      }
    }
  }

  // Game Over
  if (gameOver) {
    display.setTextSize(2);
    display.setCursor(16, 30);
    display.print("GAME OVER");
  }

  display.display();
}

void resetGame() {
  score = 0;
  distanceTravel = 0;
  speed = 2;
  shipX = 10;
  shipY = 30;
  gameOver = false;
  for (int i = 0; i < MAX_OBSTACLES; i++) {
    obstacles[i].active = false;
    obstacles[i].exploding = false;
  }
  for (int i = 0; i < MAX_BULLETS; i++) bullets[i].active = false;
}

void setupGame() {
  resetGame();
  lastBulletTime = millis();
  lastSpawnTime = millis();
}

void setup() {
  Wire.begin(12, 14);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.clearDisplay();

  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  showLoadingScreen();  // Nur beim Start!
  setupGame();
}

void loop() {
  readMPU();
  if (!gameOver) {
    updateGame();
  } else {
    delay(2000);
    setupGame(); // Restart ohne Loading Screen
  }
  drawGame();
  delay(30);
}
