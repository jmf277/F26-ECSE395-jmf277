#include <Arduino.h>
// jmf277: Extra credit for the in-class task: continuously increase and decrease the motor speed in a loop.
// jmf277: Build and upload it with the PlatformIO environment `tt_motor_ec` (see platformio.ini).

// jmf277: The L9110 motor-driver module's Motor A inputs are wired to Feather A0 (GPIO 26) and A1 (GPIO 25).
// jmf277: The motor is in the driver's Motor A terminal block, so the Feather drives the A-channel inputs
// jmf277: A-1A and A-1B (the two header pins beyond VCC). The course's TT Motor.md allows this ("You can change
// jmf277: into A-1A if you connect the driver to the motor in A"); the A and B channels behave identically.
const int MOTOR_A_1A = A0; // jmf277: A-1A
const int MOTOR_A_1B = A1; // jmf277: A-1B
// jmf277: The module's VCC and GND go to the breadboard power module's 3.3 V rail (fed by a 9 V battery), as the
// jmf277: handout's Figure 2 shows, and the module's - rail is also wired to Feather GND so the driver's inputs share a ground.

// jmf277: analogWrite() is an 8-bit, 1 kHz PWM signal, so the duty cycle runs from 0 to 255.
const int MAX_DUTY = 255;
const int DUTY_STEP = 1;                  // jmf277: change the duty cycle one count at a time
const unsigned long STEP_DELAY_MS = 12;   // jmf277: 256 steps x 12 ms = about 3 s from stopped to full speed
const unsigned long PAUSE_AT_ZERO_MS = 500; // jmf277: short rest before the direction reverses

// jmf277: Drive the motor from one signed number: the sign picks the direction and the
// jmf277: magnitude (0-255) is the PWM duty cycle on the driven input; the other input stays low.
void driveMotor(int signedSpeed) {
  int duty = constrain(abs(signedSpeed), 0, MAX_DUTY);
  if (signedSpeed >= 0) {
    analogWrite(MOTOR_A_1B, 0);     // jmf277: clockwise: A-1B low, PWM on A-1A
    analogWrite(MOTOR_A_1A, duty);
  } else {
    analogWrite(MOTOR_A_1A, 0);     // jmf277: counterclockwise: A-1A low, PWM on A-1B
    analogWrite(MOTOR_A_1B, duty);
  }
}

// jmf277: Sweep the duty cycle linearly from `from` to `to` (a triangle-wave ramp), reporting progress.
void rampSpeed(int direction, int from, int to) {
  int step = (to >= from) ? DUTY_STEP : -DUTY_STEP;
  for (int duty = from; (step > 0) ? duty <= to : duty >= to; duty += step) {
    driveMotor(direction * duty);
    if (duty % 64 == 0 || duty == MAX_DUTY) {  // jmf277: print at 0, 64, 128, 192 and 255 so the monitor stays readable
      Serial.printf("  duty %3d/255 (%3d %%)\n", duty, duty * 100 / MAX_DUTY);
    }
    delay(STEP_DELAY_MS);
  }
}

void setup() {
  Serial.begin(115200);         // jmf277: match monitor_speed in platformio.ini
  pinMode(MOTOR_A_1A, OUTPUT);  // jmf277: the ESP32 drives both L9110 inputs
  pinMode(MOTOR_A_1B, OUTPUT);
  driveMotor(0);                // jmf277: start from rest
  Serial.println("TT Motor EC: ramp up and down clockwise, then ramp up and down counterclockwise, forever");
}

void loop() {
  Serial.println("Clockwise: speeding up");
  rampSpeed(+1, 0, MAX_DUTY);        // jmf277: 0 -> 255 clockwise
  Serial.println("Clockwise: slowing down");
  rampSpeed(+1, MAX_DUTY, 0);        // jmf277: 255 -> 0 clockwise
  delay(PAUSE_AT_ZERO_MS);           // jmf277: rest at zero before reversing

  Serial.println("Counterclockwise: speeding up");
  rampSpeed(-1, 0, MAX_DUTY);        // jmf277: 0 -> 255 counterclockwise
  Serial.println("Counterclockwise: slowing down");
  rampSpeed(-1, MAX_DUTY, 0);        // jmf277: 255 -> 0 counterclockwise
  delay(PAUSE_AT_ZERO_MS);           // jmf277: rest at zero, then loop() repeats the whole cycle
}

// jmf277: A DC motor needs a minimum duty cycle to overcome friction, so the shaft stays still
// jmf277: (and may hum) for the first part of each ramp up and stops early on each ramp down.
