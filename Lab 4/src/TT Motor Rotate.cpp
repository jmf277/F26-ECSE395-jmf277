#include <Arduino.h>
// jmf277: Completed from the course's TT Motor Rotate.cpp starter for the second half of the in-class task.
// jmf277: Build and upload it with the PlatformIO environment `tt_motor_rotate` (see platformio.ini).

// jmf277: The L9110 motor-driver module's Motor A inputs are wired to Feather A0 (GPIO 26) and A1 (GPIO 25).
// jmf277: The motor is in the driver's Motor A terminal block, so the Feather drives the A-channel inputs
// jmf277: A-1A and A-1B (the two header pins beyond VCC). The course's TT Motor.md allows this ("You can change
// jmf277: into A-1A if you connect the driver to the motor in A"); the A and B channels behave identically.
const int MOTOR_A_1A = A0; // jmf277: A-1A; the starter named this MOTOR_B_1A with the placeholder 0
const int MOTOR_A_1B = A1; // jmf277: A-1B; the starter named this MOTOR_B_1B with the placeholder 0
// jmf277: The module's VCC and GND go to the breadboard power module's 3.3 V rail (fed by a 9 V battery), as the
// jmf277: handout's Figure 2 shows, and the module's - rail is also wired to Feather GND so the driver's inputs share a ground.

// jmf277: Step durations of the sequence, in milliseconds.
const unsigned long RUN_TIME_MS = 5000;  // jmf277: each rotation lasts 5 s
const unsigned long STOP_TIME_MS = 2000; // jmf277: each stop lasts 2 s

void setup() {
  Serial.begin(115200);         // jmf277: start serial output at the 115200 baud set as monitor_speed in platformio.ini

  pinMode(MOTOR_A_1A, OUTPUT);  // jmf277: the ESP32 drives A-1A
  pinMode(MOTOR_A_1B, OUTPUT);  // jmf277: the ESP32 drives A-1B

  Serial.println("TT Motor Rotate: clockwise 5 s, stop 2 s, counterclockwise 5 s, stop 2 s, then repeat"); // jmf277: describe the sequence once at start-up
}

void loop() {
  // --- SECTION 1: Clockwise (5s) ---
  Serial.println("Clockwise");     // jmf277: report the step in the serial monitor

  digitalWrite(MOTOR_A_1A, HIGH);  // jmf277: A-1A high and ...
  digitalWrite(MOTOR_A_1B, LOW);   // jmf277: ... A-1B low: the driver pushes current through the motor in one direction

  delay(RUN_TIME_MS);              // jmf277: keep turning for 5000 ms

  // --- SECTION 2: Stop (2s) ---
  Serial.println("Stop");          // jmf277: report the stop

  digitalWrite(MOTOR_A_1A, LOW);   // jmf277: both inputs low: the driver holds both motor terminals at the same level ...
  digitalWrite(MOTOR_A_1B, LOW);   // jmf277: ... so no current drives the motor and it brakes to a stop (the L9110 table's Brake state)

  delay(STOP_TIME_MS);             // jmf277: stay stopped for 2000 ms

  // --- SECTION 3: Counterclockwise (5s) ---
  Serial.println("Counterclockwise"); // jmf277: report the step

  digitalWrite(MOTOR_A_1A, LOW);   // jmf277: A-1A low and ...
  digitalWrite(MOTOR_A_1B, HIGH);  // jmf277: ... A-1B high: the current is reversed, so the motor turns the other way

  delay(RUN_TIME_MS);              // jmf277: keep turning for 5000 ms

  // --- SECTION 4: Stop (2s) ---
  Serial.println("Stop");          // jmf277: report the stop

  digitalWrite(MOTOR_A_1A, LOW);   // jmf277: both inputs low again ...
  digitalWrite(MOTOR_A_1B, LOW);   // jmf277: ... so the motor brakes to a stop

  delay(STOP_TIME_MS);             // jmf277: stay stopped for 2000 ms, then loop() runs the sequence again
}

// jmf277: "Clockwise" is the direction the shaft turns with my motor wires in the Motor A terminal
// jmf277: block. If the two motor wires are swapped in the terminal block, the directions swap too.
