#include <Arduino.h>

// put function declarations here:
int x;

void setup() {
  // put your setup code here, to run once:
  x = 0;
  Serial.begin(9600);
  Serial.println("Hello!");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(x); // Print "Hello, world!" to serial monitor
  x++;
  delay(100); // Wait for 1 second
}
