#include <SKK_nano_mcp.h>

void setup()
{
  OLED_begin();
}

void loop()
{
  int val = analogRead(A7);

  OLED.clearDisplay();
  OLED.setTextColor(WHITE, BLACK);
  OLED.setTextSize(2);
  OLED.setCursor(0, 0);
  OLED.println(F("A7"));

  OLED.setTextSize(1);
  OLED.print(F("Value = "));
  OLED.println(val);
  OLED.display();

  delay(100);
}
