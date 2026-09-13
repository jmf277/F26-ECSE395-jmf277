/*
// jmf277: Completed from the course's value.cpp starter and renamed as the handout requests.
// jmf277: Enable only one sensor sketch at a time; keep the other .cpp files block-commented.
#include <Arduino.h>

// jmf277: Feather A1 is GPIO 25 and receives the potentiometer module's OUT/AO signal.
const int sensorPin = A1;

void setup() {
    // jmf277: Start serial output at the monitor's configured 115200 baud.
    Serial.begin(115200);

    // jmf277: Select 12-bit readings, giving 4096 possible integer codes from 0 through 4095.
    analogReadResolution(12);

    // jmf277: Use the widest ADC attenuation range for the potentiometer powered from 3.3 V.
    analogSetPinAttenuation(sensorPin, ADC_11db);
}

void loop() {
    // jmf277: Read one raw ADC value from the potentiometer's signal pin.
    int sensorValue = analogRead(sensorPin);

    // jmf277: Print the integer reading so rotating the knob can be checked in the serial monitor.
    Serial.print("ADC: ");
    Serial.println(sensorValue);

    // jmf277: Wait 100 milliseconds between readings, giving approximately ten updates per second.
    delay(100);
}
*/
