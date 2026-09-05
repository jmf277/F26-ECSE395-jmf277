#include <Arduino.h>
// jmf277: Required for Arduino functions when using PlatformIO.

#define LED_PIN 13

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200); // jmf277: Start serial communication at 115200 baud.
}

void loop() {
  Serial.println("Joe's ESP32 says hello!"); // jmf277: Print the custom message once per blink cycle.

  digitalWrite(LED_PIN, HIGH); // jmf277: Turn the onboard red LED on.
  delay(250); // jmf277: Keep the LED on for 250 milliseconds (0.25 seconds).

  digitalWrite(LED_PIN, LOW); // jmf277: Turn the LED off.
  delay(1250); // jmf277: Keep the LED off for 1250 milliseconds (1.25 seconds).
}