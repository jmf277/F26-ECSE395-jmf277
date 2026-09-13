// jmf277: Adapted from the touch example shown in the Lab 3 introduction video.
// jmf277: Enable only one sensor sketch at a time; keep the other .cpp files block-commented.
#include <Arduino.h>
// jmf277: Include the Arduino functions used by this PlatformIO C++ program.

// jmf277: Read the touch module's digital output on Feather A1, which is GPIO 25.
const int sensorPin = A1;

// jmf277: GPIO 13 controls the Feather ESP32 V2's onboard red LED.
const int ledPin = 13;

void setup() {
    // jmf277: Start serial output at the same 115200 baud configured for the monitor.
    Serial.begin(115200);

    // jmf277: Configure A1 as an input for the touch module's HIGH/LOW signal.
    pinMode(sensorPin, INPUT);

    // jmf277: Configure the onboard LED pin as an output so the program can control it.
    pinMode(ledPin, OUTPUT);
}

void loop() {
    // jmf277: Sample the module's current digital state: HIGH when touched, LOW when released.
    int sensorValue = digitalRead(sensorPin);

    if (sensorValue == HIGH) {
        // jmf277: Report contact and turn the onboard LED on.
        Serial.println("Touch detected!");
        digitalWrite(ledPin, HIGH);
    } else {
        // jmf277: Report no contact and turn the onboard LED off.
        Serial.println("No touch detected...");
        digitalWrite(ledPin, LOW);
    }

    // jmf277: Wait 50 milliseconds before checking again; the LED keeps its current state meanwhile.
    delay(50);
}
