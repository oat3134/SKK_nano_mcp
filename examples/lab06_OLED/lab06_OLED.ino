#include <SKK_nano_mcp.h>

void setup()
{
  OLED_begin();

  OLED.clearDisplay();
  OLED.setTextColor(WHITE, BLACK);
  OLED.setTextSize(2);
  OLED.setCursor(20, 0);
  OLED.println(F("ROBOT"));

  OLED.setTextSize(1);
  OLED.setCursor(0, 20);
  OLED.println(F("SKK NANO MCP"));
  OLED.display();
}

void loop()
{
}
