#include <SKK_nano_mcp.h>

uint8_t button = 2;

// Threshold Sensor หน้า
uint16_t a1 = 500;
uint16_t a2 = 500;
uint16_t a3 = 500;
uint16_t a4 = 500;
uint16_t a5 = 500;
uint16_t a6 = 500;
uint16_t a7 = 500;
uint16_t a8 = 500;

// Threshold Sensor หลัง
uint16_t b1 = 500;
uint16_t b2 = 500;
uint16_t b3 = 500;
uint16_t b4 = 500;
uint16_t b5 = 500;
uint16_t b6 = 500;
uint16_t b7 = 500;
uint16_t b8 = 500;

// ค่าที่อ่านจาก Sensor
uint16_t s1,s2,s3,s4,s5,s6,s7,s8;
uint16_t sb1,sb2,sb3,sb4,sb5,sb6,sb7,sb8;

void setup()
{
  pinMode(button, INPUT);

  // Motor + OLED + MCP3008 + EEPROM Calibration
  begin(10, 3);
}

void loop()
{
  bool sw = digitalRead(button);
  uint16_t nob = analogRead(A7);
  uint8_t menu = map(nob, 0, 1000, 0, 9);

  OLED.clearDisplay();
  OLED.setTextColor(WHITE, BLACK);
  OLED.setCursor(0, 0);
  OLED.setTextSize(2);
  OLED.print(F("   "));
  OLED.println(menu);

  OLED.setTextSize(1);
  OLED.println(F("  KruRo"));
  OLED.print(F("  "));
  OLED.print(nob);
  OLED.println(F(" Robot"));
  OLED.display();

  #define mode 1

  if ((sw == mode) && (menu == 0)) menu0();
  if ((sw == mode) && (menu == 1)) menu1();
  if ((sw == mode) && (menu == 2)) menu2();
  if ((sw == mode) && (menu == 3)) menu3();
  if ((sw == mode) && (menu == 4)) menu4();
  if ((sw == mode) && (menu == 5)) menu5();
  if ((sw == mode) && (menu == 6)) menu6();
  if ((sw == mode) && (menu == 7)) menu7();
  if ((sw == mode) && (menu == 8)) menu8();
  if ((sw == mode) && (menu == 9)) menu9();

  delay(100);
}

/*
  คำสั่ง Mission

  P(100);             เดินหน้าจนเจอเส้น
  PT(100,1000);       เดินหน้าตามเวลา
  PB(100);            ถอยหลังจนเจอเส้น
  PBT(100,1000);      ถอยหลังตามเวลา
  L(100);             เลี้ยวซ้าย
  R(100);             เลี้ยวขวา
*/

void menu1()
{
  P(100);
}

void menu2()
{
  L(100);
}

void menu3()
{
  R(100);
}

void menu4()
{
  P(100);
  R(100);
  P(100);
}

void menu5()
{
  P(100);
  L(100);
  P(100);
}

void menu6()
{
  PT(100, 1000);
}

void menu7()
{
  PB(100);
}

void menu8()
{
  PBT(100, 1000);
}

void menu9()
{
  P(100);
  R(100);
  P(100);
  L(100);
  P(100);
}
