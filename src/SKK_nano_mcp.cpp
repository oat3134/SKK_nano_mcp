#include "SKK_nano_mcp.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MCP3008.h>
#include <EEPROM.h>


// ------------------------------------------------------------------
// ตัวแปรที่ยังอยู่ใน MAIN
// ------------------------------------------------------------------
Adafruit_SSD1306 OLED(-1);

uint8_t button __attribute__((weak)) = 2;

uint16_t s1 __attribute__((weak)) = 0;
uint16_t s2 __attribute__((weak)) = 0;
uint16_t s3 __attribute__((weak)) = 0;
uint16_t s4 __attribute__((weak)) = 0;
uint16_t s5 __attribute__((weak)) = 0;
uint16_t s6 __attribute__((weak)) = 0;
uint16_t s7 __attribute__((weak)) = 0;
uint16_t s8 __attribute__((weak)) = 0;

uint16_t a1 __attribute__((weak)) = 500;
uint16_t a2 __attribute__((weak)) = 500;
uint16_t a3 __attribute__((weak)) = 500;
uint16_t a4 __attribute__((weak)) = 500;
uint16_t a5 __attribute__((weak)) = 500;
uint16_t a6 __attribute__((weak)) = 500;
uint16_t a7 __attribute__((weak)) = 500;
uint16_t a8 __attribute__((weak)) = 500;

// ค่าชดเชยความเร็วมอเตอร์: main สามารถกำหนดค่าใหม่ได้
int speed_L __attribute__((weak)) = 0;
int speed_R __attribute__((weak)) = 0;

// Servo object is declared in the .ino, same style as SKKnano
extern Servo sv1;
extern uint8_t servo1;

namespace {

// Motor pins - freeze ตามบอร์ดเดิม
const uint8_t DR1  = 7;
const uint8_t DR2  = 8;
const uint8_t PWMR = 6;
const uint8_t DL1  = 4;
const uint8_t DL2  = 9;
const uint8_t PWML = 5;

Adafruit_MCP3008 adcFront;

uint16_t minFront[8];
uint16_t maxFront[8];

uint16_t defaultFront[8];

const uint16_t EEPROM_MAGIC = 0x4D43;
const int EEPROM_ADDR = 0;

struct CalibrationEEPROM {
  uint16_t magic;
  uint16_t front[8];
};

void captureDefaults()
{
  uint16_t f[8] = {a1,a2,a3,a4,a5,a6,a7,a8};

  for (byte i = 0; i < 8; i++) {
    defaultFront[i] = f[i];
  }
}

void resetFrontMinMax()
{
  for (byte i = 0; i < 8; i++) {
    minFront[i] = 1023;
    maxFront[i] = 0;
  }
}

void sampleFront()
{
  readFront();

  uint16_t v[8] = {s1,s2,s3,s4,s5,s6,s7,s8};

  for (byte i = 0; i < 8; i++) {
    if (v[i] < minFront[i]) minFront[i] = v[i];
    if (v[i] > maxFront[i]) maxFront[i] = v[i];
  }
}

void calibrationMoveFront(int left, int right, unsigned long t)
{
  unsigned long start = millis();
  run(left, right);

  while (millis() - start < t) {
    sampleFront();
  }
}

void calculateFrontThreshold()
{
  a1 = (minFront[0] + maxFront[0]) / 2;
  a2 = (minFront[1] + maxFront[1]) / 2;
  a3 = (minFront[2] + maxFront[2]) / 2;
  a4 = (minFront[3] + maxFront[3]) / 2;
  a5 = (minFront[4] + maxFront[4]) / 2;
  a6 = (minFront[5] + maxFront[5]) / 2;
  a7 = (minFront[6] + maxFront[6]) / 2;
  a8 = (minFront[7] + maxFront[7]) / 2;
}

void showCalibrationFront()
{
  while (digitalRead(button) == HIGH) {}
  delay(100);

  while (true) {
    int16_t y = map(analogRead(A7), 0, 1023, 0, -70);
    uint16_t mid[8] = {a1,a2,a3,a4,a5,a6,a7,a8};

    OLED.clearDisplay();
    OLED.setTextColor(WHITE, BLACK);
    OLED.setTextSize(1);
    OLED.setCursor(0, y);
    OLED.println(F("FRONT MIN MAX MID"));

    for (byte i = 0; i < 8; i++) {
      OLED.print(F("F")); OLED.print(i + 1);
      OLED.print(F(" ")); OLED.print(minFront[i]);
      OLED.print(F(" ")); OLED.print(maxFront[i]);
      OLED.print(F(" ")); OLED.println(mid[i]);
    }

    OLED.println(F("PRESS = EXIT"));
    OLED.display();

    if (digitalRead(button) == HIGH) {
      delay(30);
      if (digitalRead(button) == HIGH) {
        while (digitalRead(button) == HIGH) {}
        delay(100);
        return;
      }
    }
  }
}

void normalSensor()
{
  while (digitalRead(button) == HIGH) {}
  delay(100);

  while (true) {
    readFront();

    int16_t y = map(analogRead(A7), 0, 1023, 0, -180);

    OLED.clearDisplay();
    OLED.setTextColor(WHITE, BLACK);
    OLED.setCursor(0, y);
    OLED.setTextSize(1);

    OLED.print(F("       a1 = ")); OLED.println(s1);
    OLED.print(F("       a2 = ")); OLED.println(s2);
    OLED.print(F("       a3 = ")); OLED.println(s3);
    OLED.print(F("       a4 = ")); OLED.println(s4);
    OLED.print(F("       a5 = ")); OLED.println(s5);
    OLED.print(F("       a6 = ")); OLED.println(s6);
    OLED.print(F("       a7 = ")); OLED.println(s7);
    OLED.print(F("       a8 = ")); OLED.println(s8);
    OLED.println();
    OLED.display();

    if (digitalRead(button) == HIGH) {
      delay(30);
      if (digitalRead(button) == HIGH) {
        while (digitalRead(button) == HIGH) {}
        delay(100);
        return;
      }
    }

    delay(30);
  }
}

void calibrateFrontInternal()
{
  resetFrontMinMax();

  OLED.clearDisplay();
  OLED.setTextColor(WHITE, BLACK);
  OLED.setTextSize(2);
  OLED.setCursor(0, 0);
  OLED.println(F("FRONT"));
  OLED.println(F("CAL..."));
  OLED.display();
  delay(300);

  calibrationMoveFront(-100,  100, 500);
  calibrationMoveFront( 100, -100, 800);
  calibrationMoveFront(-100,  100, 800);
  calibrationMoveFront( 100, -100, 800);
  calibrationMoveFront(-100,  100, 500);

  run(0, 0);

  calculateFrontThreshold();
  saveEEPROM();
  showCalibrationFront();
}


} // namespace

void Motor_begin()
{
  pinMode(DL1, OUTPUT);
  pinMode(DL2, OUTPUT);
  pinMode(PWML, OUTPUT);
  pinMode(DR1, OUTPUT);
  pinMode(DR2, OUTPUT);
  pinMode(PWMR, OUTPUT);
}

void OLED_begin()
{
  Wire.begin();
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.clearDisplay();
  OLED.display();
}

void Sensor_begin(uint8_t frontCS, uint8_t backCS)
{
  (void)backCS;
  adcFront.begin(frontCS);
}

void begin(uint8_t frontCS, uint8_t backCS)
{
  captureDefaults();
  Motor_begin();
  OLED_begin();
  Sensor_begin(frontCS, backCS);
  loadEEPROM();
}

void readFront()
{
  s1 = adcFront.readADC(0);
  s2 = adcFront.readADC(1);
  s3 = adcFront.readADC(2);
  s4 = adcFront.readADC(3);
  s5 = adcFront.readADC(4);
  s6 = adcFront.readADC(5);
  s7 = adcFront.readADC(6);
  s8 = adcFront.readADC(7);
}

void run(int spl, int spr)
{
  if (spl > 0) spl += speed_L;
  else if (spl < 0) spl -= speed_L;

  if (spr > 0) spr += speed_R;
  else if (spr < 0) spr -= speed_R;

  spl = constrain(spl, -255, 255);
  spr = constrain(spr, -255, 255);

  if (spl > 0) {
    spl = abs(spl);
    digitalWrite(DL1, LOW);
    digitalWrite(DL2, HIGH);
    analogWrite(PWML, spl);
  }
  else if (spl < 0) {
    spl = abs(spl);
    digitalWrite(DL1, HIGH);
    digitalWrite(DL2, LOW);
    analogWrite(PWML, spl);
  }
  else {
    digitalWrite(DL1, HIGH);
    digitalWrite(DL2, HIGH);
  }

  if (spr > 0) {
    spr = abs(spr);
    digitalWrite(DR1, LOW);
    digitalWrite(DR2, HIGH);
    analogWrite(PWMR, spr);
  }
  else if (spr < 0) {
    spr = abs(spr);
    digitalWrite(DR1, HIGH);
    digitalWrite(DR2, LOW);
    analogWrite(PWMR, spr);
  }
  else {
    digitalWrite(DR1, HIGH);
    digitalWrite(DR2, HIGH);
  }
}

void motorTest()
{
  run(100,100);   delay(500); run(0,0); delay(500);
  run(-100,100);  delay(500); run(0,0); delay(500);
  run(100,-100);  delay(500); run(0,0); delay(500);
  run(-100,-100); delay(500); run(0,0); delay(500);
}


void servo()
{
  while (digitalRead(button) == HIGH) {}
  delay(100);

  while (true) {
    uint16_t vr = analogRead(A7);
    uint8_t angle = map(vr, 0, 1023, 0, 180);

    sv1.write(angle);

    OLED.clearDisplay();
    OLED.setTextColor(WHITE, BLACK);
    OLED.setCursor(0, 0);
    OLED.setTextSize(2);
    OLED.println(F("SERVO"));
    OLED.setTextSize(1);
    OLED.print(F("ANGLE = "));
    OLED.println(angle);
    OLED.println(F("PRESS = EXIT"));
    OLED.display();

    if (digitalRead(button) == HIGH) {
      delay(30);
      if (digitalRead(button) == HIGH) {
        while (digitalRead(button) == HIGH) {}
        sv1.detach();
        delay(100);
        return;
      }
    }

    delay(20);
  }
}

void P(byte speed)
{
  while (1) {
    readFront();

    if (((s1<a1) && (s2<a2)) || ((s7<a7) && (s8<a8))) {
      run(100,100);
      delay(180);
      run(0,0);
      delay(100);
      break;
    }
    else if (s2<a2) run(-40,100);
    else if (s3<a3) run(80,100);
    else if (s6<a6) run(100,80);
    else if (s7<a7) run(100,-40);
    else run(speed,speed);
  }
}

void PT(byte speed, unsigned long t)
{
  unsigned long start = millis();

  while (1) {
    readFront();

    if (millis() - start >= t) {
      run(0,0);
      break;
    }
    else if (((s1<a1) && (s2<a2)) || ((s7<a7) && (s8<a8))) run(speed,speed);
    else if (s2<a2) run(-40,100);
    else if (s3<a3) run(80,100);
    else if (s6<a6) run(100,80);
    else if (s7<a7) run(100,-40);
    else run(speed,speed);
  }
}

void B(byte speed)
{
  while (1) {
    // แก้จากโค้ดเดิม: PB ต้องอ่าน Sensor หลัง
    readFront();

    if (((s1<a1) && (s2<a2)) || ((s7<a7) && (s8<a8))) {
      run(100,100);
      delay(180);
      run(0,0);
      delay(100);
      break;
    }
    else run(-speed,-speed);
  }
}

void BT(byte speed, unsigned long t)
{
  unsigned long start = millis();

  while (1) {
    // แก้จากโค้ดเดิม: PBT ต้องอ่าน Sensor หลัง

    if (millis() - start >= t) {
      run(0,0);
      break;
    }
    else run(-speed,-speed);
  }
}

void L(byte sp)
{
  L0(sp);
  L1(sp);
  L2(sp);
  delay(100);
}

void L0(byte sp)
{
  while (1) {
    readFront();
    run(-sp,sp);
    if (s1<a1) {
      run(0,0);
      break;
    }
  }
}

void L1(byte sp)
{
  while (1) {
    readFront();
    run(-sp,sp);
    if (s2<a2) {
      run(0,0);
      break;
    }
  }
}

void L2(byte sp)
{
  while (1) {
    readFront();
    run(-sp,sp);
    if (s4<a4) {
      run(0,0);
      break;
    }
  }
}

void R(byte sp)
{
  R0(sp);
  R1(sp);
  R2(sp);
  delay(100);
}

void R0(byte sp)
{
  while (1) {
    readFront();
    run(sp,-sp);
    if (s8<a8) {
      run(0,0);
      break;
    }
  }
}

void R1(byte sp)
{
  while (1) {
    readFront();
    run(sp,-sp);
    if (s7<a7) {
      run(0,0);
      break;
    }
  }
}

void R2(byte sp)
{
  while (1) {
    readFront();
    run(sp,-sp);
    if (s5<a5) {
      run(0,0);
      break;
    }
  }
}

void saveEEPROM()
{
  CalibrationEEPROM data;
  data.magic = EEPROM_MAGIC;

  uint16_t f[8] = {a1,a2,a3,a4,a5,a6,a7,a8};

  for (byte i = 0; i < 8; i++) {
    data.front[i] = f[i];
  }

  EEPROM.put(EEPROM_ADDR, data);
}

void loadEEPROM()
{
  CalibrationEEPROM data;
  EEPROM.get(EEPROM_ADDR, data);

  if (data.magic != EEPROM_MAGIC) return;

  for (byte i = 0; i < 8; i++) {
    if (data.front[i] > 1023) return;
  }

  a1=data.front[0]; a2=data.front[1]; a3=data.front[2]; a4=data.front[3];
  a5=data.front[4]; a6=data.front[5]; a7=data.front[6]; a8=data.front[7];

}

void resetCalibration()
{
  a1=defaultFront[0]; a2=defaultFront[1]; a3=defaultFront[2]; a4=defaultFront[3];
  a5=defaultFront[4]; a6=defaultFront[5]; a7=defaultFront[6]; a8=defaultFront[7];

  saveEEPROM();

  OLED.clearDisplay();
  OLED.setTextColor(WHITE, BLACK);
  OLED.setCursor(0, 0);
  OLED.setTextSize(2);
  OLED.println(F("RESET"));
  OLED.println(F("DONE"));
  OLED.setTextSize(1);
  OLED.println(F("DEFAULT SAVED"));
  OLED.display();

  delay(1000);
}

void menu0()
{
  while (digitalRead(button) == HIGH) {}
  delay(120);

  while (true) {
    uint8_t select = map(analogRead(A7), 0, 1023, 0, 1);

    OLED.clearDisplay();
    OLED.setTextColor(WHITE, BLACK);
    OLED.setCursor(0, 0);
    OLED.setTextSize(1);
    OLED.println(F("SENSOR MODE"));
    OLED.println();
    OLED.setTextSize(2);

    if (select == 0) OLED.println(F("> NORMAL"));
    else             OLED.println(F("> FRONT"));

    OLED.setTextSize(1);
    OLED.println(F("PRESS = ENTER"));
    OLED.display();

    if (digitalRead(button) == HIGH) {
      delay(30);

      if (digitalRead(button) == HIGH) {
        while (digitalRead(button) == HIGH) {}
        delay(100);

        if (select == 0) normalSensor();
        else             calibrateFrontInternal();

        return;
      }
    }

    delay(30);
  }
}
