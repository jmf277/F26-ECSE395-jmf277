/*
// jmf277: Completed from the course's voltage.cpp starter using the lab's voltage-estimation formula.
// jmf277: Enable only one sensor sketch at a time; keep the other .cpp files block-commented.
#include <Arduino.h>

// jmf277: Declare a floating-point conversion function so fractional voltages are preserved.
float voltage(float analogValue);

// jmf277: Reuse Feather A1/GPIO 25, so the potentiometer wiring stays the same as the raw-reading task.
const int sensorPin = A1;

// jmf277: Use the nominal 3.3 V sensor supply for the lab's estimate; this is not ADC calibration.
const float referenceVoltage = 3.3f;

// jmf277: A 12-bit reading has 4096 codes; 4095 is the maximum code used for endpoint scaling.
const float adcMaximum = 4095.0f;

void setup() {
    // jmf277: Start serial output at the monitor's configured 115200 baud.
    Serial.begin(115200);

    // jmf277: Keep the raw reading range consistent with the conversion denominator.
    analogReadResolution(12);

    // jmf277: Use the same ADC attenuation as the raw-reading program; near-rail saturation is possible.
    analogSetPinAttenuation(sensorPin, ADC_11db);
}

void loop() {
    // jmf277: Store the raw analog reading in an integer before converting or printing it.
    int sensorValue = analogRead(sensorPin);

    // jmf277: Convert the stored reading to an estimated voltage using floating-point arithmetic.
    float sensorVoltage = voltage(sensorValue);

    // jmf277: Show both the source reading and its estimated voltage, with three decimal places and units.
    Serial.print("ADC: ");
    Serial.print(sensorValue);
    Serial.print(" | Estimated voltage: ");
    Serial.print(sensorVoltage, 3);
    Serial.println(" V");

    // jmf277: Change the starter's 50 ms delay to 200 ms for approximately five readable updates per second.
    delay(200);
}

// jmf277: Map the nominal 0..4095 input range to 0..3.3 V; hardware accuracy must be checked separately.
float voltage(float analogValue) {
    float outputVoltage = (analogValue * referenceVoltage) / adcMaximum;
    return outputVoltage;
}
*/
