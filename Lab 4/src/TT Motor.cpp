#include <Arduino.h>
// jmf277: Completed from the course's TT Motor.cpp starter for the first half of the in-class task.
// jmf277: Build and upload it with the PlatformIO environment `tt_motor` (see platformio.ini).

// jmf277: The L9110 motor-driver module's two Motor A inputs are wired to Feather A0 and A1.
// jmf277: A0 is GPIO 26 and A1 is GPIO 25; both can drive an output, unlike the input-only A2-A4.
// jmf277: The motor is in the driver's Motor A terminal block, so the Feather drives the A-channel inputs
// jmf277: A-1A and A-1B (the two header pins beyond VCC). The course's TT Motor.md allows this ("You can change
// jmf277: into A-1A if you connect the driver to the motor in A"); the A and B channels behave identically.
const int MOTOR_A_1A = A0; // jmf277: A-1A; the starter named this MOTOR_B_1A with the placeholder 0
const int MOTOR_A_1B = A1; // jmf277: A-1B; the starter named this MOTOR_B_1B with the placeholder 0
// jmf277: The module's VCC and GND go to the breadboard power module's 3.3 V rail (fed by a 9 V battery), as the
// jmf277: handout's Figure 2 shows, and the module's - rail is also wired to Feather GND so the driver's inputs share a ground.

// jmf277: The in-class task changes three things ONE AT A TIME and observes the motor:
// jmf277:   Trial 1  starter values        A-1A = 255, A-1B = 0,   run for 5000 ms
// jmf277:   Trial 2  analogWrite() value   A-1A = 128, A-1B = 0,   run for 5000 ms
// jmf277:   Trial 3  swapped values        A-1A = 0,   A-1B = 255, run for 5000 ms
// jmf277:   Trial 4  delay() value         A-1A = 255, A-1B = 0,   run for 2000 ms
// jmf277: Set TRIAL to 1, 2, 3 or 4, then build and upload. Each case in configureTrial()
// jmf277: changes exactly one thing relative to the starter, so the trials stay comparable.
const int TRIAL = 4;

// jmf277: analogWrite() on the ESP32 Arduino core is an 8-bit, 1 kHz PWM signal:
// jmf277: 255 keeps the input high all the time (full speed) and 128 is about a 50 % duty cycle.
int speedA = 255;              // jmf277: PWM value written to A-1A (starter value 255)
int speedB = 0;                // jmf277: PWM value written to A-1B (starter value 0)
unsigned long runTimeMs = 5000; // jmf277: how long the motor runs before stopping (starter value 5000)

void configureTrial() {
  switch (TRIAL) {
    case 2:
      speedA = 128;      // jmf277: Trial 2 changes the analogWrite() value from 255 to 128
      break;
    case 3:
      speedA = 0;        // jmf277: Trial 3 swaps the two analogWrite() values ...
      speedB = 255;      // jmf277: ... so A-1B is driven instead of A-1A
      break;
    case 4:
      runTimeMs = 2000;  // jmf277: Trial 4 changes the delay() from 5000 ms to 2000 ms
      break;
    default:
      break;             // jmf277: Trial 1 keeps the starter values
  }
}

void setup() {
  Serial.begin(115200);  // jmf277: match the monitor_speed in platformio.ini so the trial is visible in the monitor
  configureTrial();      // jmf277: apply the one change selected by TRIAL

  pinMode(MOTOR_A_1A, OUTPUT);
  pinMode(MOTOR_A_1B, OUTPUT);

  Serial.printf("TT Motor trial %d: A-1A = %d, A-1B = %d for %lu ms\n", TRIAL, speedA, speedB, runTimeMs);

  analogWrite(MOTOR_A_1A, speedA); // jmf277: starter wrote 255 here; Trial 2 uses 128, Trial 3 uses 0
  analogWrite(MOTOR_A_1B, speedB); // jmf277: starter wrote 0 here; Trial 3 uses 255

  delay(runTimeMs);                // jmf277: starter waited 5000 ms; Trial 4 waits 2000 ms

  analogWrite(MOTOR_A_1A, 0);      // jmf277: both inputs low: the driver holds both motor terminals at the same level,
  analogWrite(MOTOR_A_1B, 0);      // jmf277: so no current drives the motor and it brakes to a stop (the L9110 table's Brake state)
  Serial.println("Motor stopped. Press RESET on the Feather to run the trial again.");
}

void loop() {
  // jmf277: Nothing here on purpose: the starter runs the motor once in setup().
  // jmf277: Pressing the RESET button on the ESP32 runs the selected trial again.
}
