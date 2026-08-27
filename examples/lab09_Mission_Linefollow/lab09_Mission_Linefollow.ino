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

// ค่าที่อ่านจาก Sensor
uint16_t s1,s2,s3,s4,s5,s6,s7,s8;

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
  uint8_t menu = map(nob, 0, 1023, 0, 5);

  OLED.clearDisplay();
  OLED.setTextColor(WHITE, BLACK);
  OLED.setCursor(0, 0);
  OLED.setTextSize(2);
  OLED.print(F("   "));
  OLED.println(menu);

  OLED.setTextSize(1);
  OLED.println(F("  SKK-UTHAI"));
  OLED.print(F("  "));
  OLED.print(nob);
  OLED.println(F(" Robot"));
  OLED.display();

  #define mode 1

  if ((sw == mode) && (menu == 0)) menu0(); //senser
  if ((sw == mode) && (menu == 1)) menu1(); //servo
  if ((sw == mode) && (menu == 2)) menu2();
  if ((sw == mode) && (menu == 3)) menu3();
  if ((sw == mode) && (menu == 4)) menu4();
  if ((sw == mode) && (menu == 5)) menu5();

  delay(100);
}

/*
  คำสั่ง Mission

  P(100);             เดินหน้าจนเจอเส้น
  PT(100,1000);       เดินหน้าตามเวลา
  B(100);            ถอยหลังจนเจอเส้น
  BT(100,1000);      ถอยหลังตามเวลา
  L(100);             เลี้ยวซ้าย
  R(100);             เลี้ยวขวา
*/

// ชดเชยความเร็วมอเตอร์ซ้าย/ขวา
// ค่าบวก = เพิ่ม PWM ให้ล้อนั้น
int speed_L = 0;
int speed_R = 0;

void menu1()
{
  servo();
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