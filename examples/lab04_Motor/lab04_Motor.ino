#include <SKK_nano_mcp.h>

#define buttonPin 2

void setup()
{
  pinMode(buttonPin, INPUT);
  Motor_begin();
}

void loop()
{
  int sw = digitalRead(buttonPin);

  if (sw == 1) {
    run(200, 200);
    delay(500);
    run(0, 0);
    delay(500);

    run(-200, 200);
    delay(500);
    run(0, 0);
    delay(500);

    run(200, -200);
    delay(500);
    run(0, 0);
    delay(500);

    run(-200, -200);
    delay(500);
    run(0, 0);
  }
}
