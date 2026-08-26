#include <SKK_nano_mcp.h>

int analogPin = A7;
int val = 0;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  val = analogRead(analogPin);
  Serial.println(val);
  delay(100);
}
