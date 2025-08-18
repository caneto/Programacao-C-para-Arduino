#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MPU 0x68

int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

float xx, xy, xz;
float yx, yy, yz;
float zx, zy, zz;

float fact;
int Xan, Yan;
int Xoff = 80;
int Yoff = 40;
int Zoff = 450;

struct Point3d { int x, y, z; };
struct Point2d { int x, y; };
struct Line3d { Point3d p0, p1; };
struct Line2d { Point2d p0, p1; };

Line3d Lines[12];
Line2d Render[12], ORender[12];
int LinestoRender = 12;
int OldLinestoRender = 12;

void SetVars() {
  float Xan2 = Xan / fact;
  float Yan2 = Yan / fact;
  float s1 = sin(Yan2), s2 = sin(Xan2);
  float c1 = cos(Yan2), c2 = cos(Xan2);

  xx = c1;  xy = 0;   xz = -s1;
  yx = s1 * s2;  yy = c2;  yz = c1 * s2;
  zx = s1 * c2;  zy = -s2; zz = c1 * c2;
}

void ProcessLine(Line2d *ret, Line3d vec) {
  int xv1 = vec.p0.x * xx + vec.p0.y * xy + vec.p0.z * xz;
  int yv1 = vec.p0.x * yx + vec.p0.y * yy + vec.p0.z * yz;
  int zv1 = vec.p0.x * zx + vec.p0.y * zy + vec.p0.z * zz;
  float zvt1 = zv1 - Zoff;

  int xv2 = vec.p1.x * xx + vec.p1.y * xy + vec.p1.z * xz;
  int yv2 = vec.p1.x * yx + vec.p1.y * yy + vec.p1.z * yz;
  int zv2 = vec.p1.x * zx + vec.p1.y * zy + vec.p1.z * zz;
  float zvt2 = zv2 - Zoff;

  if (zvt1 < -5 && zvt2 < -5) {
    ret->p0.x = 256 * (xv1 / zvt1) + Xoff;
    ret->p0.y = 256 * (yv1 / zvt1) + Yoff;
    ret->p1.x = 256 * (xv2 / zvt2) + Xoff;
    ret->p1.y = 256 * (yv2 / zvt2) + Yoff;
  }
}

void setupCube() {
  int s = 20;
  Lines[0] = {{-s,-s, s}, { s,-s, s}};
  Lines[1] = {{ s,-s, s}, { s, s, s}};
  Lines[2] = {{ s, s, s}, {-s, s, s}};
  Lines[3] = {{-s, s, s}, {-s,-s, s}};
  Lines[4] = {{-s,-s,-s}, { s,-s,-s}};
  Lines[5] = {{ s,-s,-s}, { s, s,-s}};
  Lines[6] = {{ s, s,-s}, {-s, s,-s}};
  Lines[7] = {{-s, s,-s}, {-s,-s,-s}};
  Lines[8] = {{-s,-s, s}, {-s,-s,-s}};
  Lines[9] = {{ s,-s, s}, { s,-s,-s}};
  Lines[10]= {{-s, s, s}, {-s, s,-s}};
  Lines[11]= {{ s, s, s}, { s, s,-s}};
}

void initMPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void readMPU() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);
  AcX = Wire.read()<<8 | Wire.read();
  AcY = Wire.read()<<8 | Wire.read();
  AcZ = Wire.read()<<8 | Wire.read();
  Tmp = Wire.read()<<8 | Wire.read();
  GyX = Wire.read()<<8 | Wire.read();
  GyY = Wire.read()<<8 | Wire.read();
  GyZ = Wire.read()<<8 | Wire.read();
}

void drawCube() {
  for (int i = 0; i < OldLinestoRender; i++) {
    display.drawLine(ORender[i].p0.x, ORender[i].p0.y, ORender[i].p1.x, ORender[i].p1.y, BLACK);
  }
  for (int i = 0; i < LinestoRender; i++) {
    display.drawLine(Render[i].p0.x, Render[i].p0.y, Render[i].p1.x, Render[i].p1.y, WHITE);
  }
  OldLinestoRender = LinestoRender;
}

void setup() {
  Wire.begin(); // 12, 14
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  display.clearDisplay();

  fact = 180 / 3.14159265359;
  setupCube();
  initMPU();
}

void loop() {
  readMPU();
  display.clearDisplay();

  int xOut = map(AcX, -30000, 30000, -50, 50);
  int yOut = map(AcY, -30000, 30000, -50, 50);

  Xan += xOut;
  Yan += yOut;
  Xan %= 360;
  Yan %= 360;

  SetVars();

  for (int i = 0; i < LinestoRender; i++) {
    ORender[i] = Render[i];
    ProcessLine(&Render[i], Lines[i]);
  }

  drawCube();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("AcX: "); display.println(AcX);
  display.print("AcY: "); display.println(AcY);
  display.print("GyX: "); display.println(GyX);
  display.print("GyY: "); display.println(GyY);

  display.display();
  delay(50);
}