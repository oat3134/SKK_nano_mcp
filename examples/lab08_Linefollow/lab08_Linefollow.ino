#include <SKK_nano_mcp.h>

uint8_t button = 2;

// ค่ากลาง Sensor หน้า
uint16_t a1 = 500;
uint16_t a2 = 500;
uint16_t a3 = 500;
uint16_t a4 = 500;
uint16_t a5 = 500;
uint16_t a6 = 500;
uint16_t a7 = 500;
uint16_t a8 = 500;

// ค่าที่อ่านจาก MCP3008 ตัวหน้า
uint16_t s1,s2,s3,s4,s5,s6,s7,s8;

void setup()
{
  pinMode(button, INPUT);
  Motor_begin();
  Sensor_begin(10, 3);
}

void loop()
{
  int sw = digitalRead(button);

  if (sw == 1) {
    while (true) {

      readFront();

      // เจอเส้นคู่ด้านซ้ายหรือขวา -> หยุด
      if (((s1 < a1) && (s2 < a2)) ||
          ((s7 < a7) && (s8 < a8))) {
        run(0, 0);
        delay(100);
        break;
      }

      else if (s2 < a2) {
        run(-40, 100);
      }
      else if (s3 < a3) {
        run(80, 100);
      }
      else if (s6 < a6) {
        run(100, 80);
      }
      else if (s7 < a7) {
        run(100, -40);
      }
      else {
        run(100, 100);
      }
    }
  }
}
