#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

#ifndef SKK_NANO_MCP_H
#define SKK_NANO_MCP_H


// เริ่ม Motor + MCP3008 + โหลด Threshold จาก EEPROM
extern Adafruit_SSD1306 OLED;

void begin(uint8_t frontCS = 10, uint8_t backCS = 3);

// เริ่มเฉพาะส่วน ใช้สำหรับ LAB
void Motor_begin();
void OLED_begin();
void Sensor_begin(uint8_t frontCS = 10, uint8_t backCS = 3);

// MENU 0: NORMAL / FRONT / RESET
void menu0();

// Motor
extern int speed_L;
extern int speed_R;
void run(int spl, int spr);
void motorTest();
void servo(uint8_t pin);

// เดินหน้า / ถอยหลัง
void P(byte speed);
void PT(byte speed, unsigned long t);
void B(byte speed);
void BT(byte speed, unsigned long t);

// เลี้ยวซ้าย
void L(byte sp = 150);
void L0(byte sp = 150);
void L1(byte sp = 150);
void L2(byte sp = 150);

// เลี้ยวขวา
void R(byte sp = 150);
void R0(byte sp = 150);
void R1(byte sp = 150);
void R2(byte sp = 150);

// ถ้าต้องการอ่าน Sensor จาก main โดยตรง
void readFront();

// EEPROM
void saveEEPROM();
void loadEEPROM();
void resetCalibration();


#endif
