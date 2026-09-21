#include <ESP32Servo.h>
// jmf277: Completed from the course's Servo Motor.cpp starter for the first half of the post-class task.
// jmf277: Build and upload it with the PlatformIO environment `servo_motor` (see platformio.ini).
// jmf277: The ESP32Servo library is installed automatically by the lib_deps line in platformio.ini.

// jmf277: The servo's orange signal wire is on Feather A0 (GPIO 26), an output-capable PWM pin. Red and
// jmf277: brown go to the breadboard power module's 5 V rail (fed by a 9 V battery), as the handout's Figure 3 shows,
// jmf277: and the module's - rail is also wired to Feather GND so the signal and the servo share a ground.
Servo myServo;
const int servoPin = A0; // jmf277: the starter used the placeholder 0

// jmf277: The post-class task changes five things ONE AT A TIME and observes the servo:
// jmf277:   Trial 1  starter values     min 500 us, max 2500 us, 50 Hz, sweep 0-180 deg, 15 ms per degree
// jmf277:   Trial 2  minPulseWidth      500 -> 1000 us
// jmf277:   Trial 3  maxPulseWidth      2500 -> 2000 us
// jmf277:   Trial 4  setPeriodHertz     50 -> 100 Hz
// jmf277:   Trial 5  rotation range     0-180 -> 45-135 deg
// jmf277:   Trial 6  delay              15 -> 5 ms per degree
// jmf277: Set TRIAL to 1-6, then build and upload. Each case in configureTrial() changes exactly
// jmf277: one parameter relative to the starter, so the trials stay comparable.
const int TRIAL = 6;

// jmf277: Servo parameters, starting from the starter's values; configureTrial() changes one of them.
int minPulseWidth = 500;   // jmf277: pulse width for 0 degrees, in microseconds (0.5 ms)
int maxPulseWidth = 2500;  // jmf277: pulse width for 180 degrees, in microseconds (2.5 ms)
int periodHertz = 50;      // jmf277: how many pulses per second the servo receives (standard 50 Hz = one every 20 ms)
int sweepStartAngle = 0;   // jmf277: rotation range, first angle of the sweep
int sweepEndAngle = 180;   // jmf277: rotation range, last angle of the sweep
int stepDelayMs = 15;      // jmf277: pause after each 1-degree step

void configureTrial() {
  switch (TRIAL) {
    case 2: minPulseWidth = 1000; break;             // jmf277: Trial 2 changes only minPulseWidth
    case 3: maxPulseWidth = 2000; break;             // jmf277: Trial 3 changes only maxPulseWidth
    case 4: periodHertz = 100; break;                // jmf277: Trial 4 changes only setPeriodHertz
    case 5: sweepStartAngle = 45; sweepEndAngle = 135; break; // jmf277: Trial 5 changes only the rotation range
    case 6: stepDelayMs = 5; break;                  // jmf277: Trial 6 changes only the delay
    default: break;                                  // jmf277: Trial 1 keeps the starter values
  }
}

// jmf277: Convert an angle to a pulse width and send it, printing every 15th degree so the monitor stays readable.
void moveTo(int angle) {
  // the map function maps 0 degrees to minPulseWidth and 180 degrees to maxPulseWidth:
  // map(angle, min angle, max angle, min pulse width, max pulse width)
  int pulseWidth = map(angle, 0, 180, minPulseWidth, maxPulseWidth);
  myServo.writeMicroseconds(pulseWidth);
  if (angle % 15 == 0) {
    Serial.printf("Angle: %d  pulse: %d us\n", angle, pulseWidth);
  }
}

void setup() {
  Serial.begin(115200);  // jmf277: match monitor_speed in platformio.ini
  configureTrial();      // jmf277: apply the one change selected by TRIAL
  Serial.printf("Servo Motor trial %d: min %d us, max %d us, %d Hz, sweep %d-%d deg, %d ms per step\n",
                TRIAL, minPulseWidth, maxPulseWidth, periodHertz, sweepStartAngle, sweepEndAngle, stepDelayMs);

  // Set the PWM frequency for the servo
  // jmf277: The starter called this after attach(). ESP32Servo's own examples call it first, because
  // jmf277: attach() reads the period when it configures the PWM timer; calling it afterwards also works
  // jmf277: (the library detaches and re-attaches the pin), but setting it first avoids that extra step.
  myServo.setPeriodHertz(periodHertz); // standard 50 Hz servo unless Trial 4 changes it

  // Attach the servo to the specified pin and set its pulse width range
  // jmf277: The library clamps min to at least 500 us and max to at most 2500 us; writeMicroseconds()
  // jmf277: clamps every command into [min, max], so the sweep can never leave this range.
  myServo.attach(servoPin, minPulseWidth, maxPulseWidth);
}

void loop() {
  // Rotate the servo from the start of the range to the end, one degree at a time
  // jmf277: The starter's loops ran 0..180; these bounds come from the trial's rotation range (Trial 5 uses 45..135).
  for (int angle = sweepStartAngle; angle <= sweepEndAngle; angle++) {
    moveTo(angle);
    delay(stepDelayMs);
  }

  // Rotate the servo back from the end of the range to the start
  // jmf277: The starter's second loop was `for (angle = 180; angle <= 0; angle--)`, a condition that is false
  // jmf277: immediately, so the return sweep never ran and the servo snapped back to 0 at the top of the
  // jmf277: next loop(). Changing `<=` to `>=` makes the servo sweep back as the handout describes. The Lab 4
  // jmf277: intro video calls out this fix (line 33 of the starter) and the updated course template has it too.
  for (int angle = sweepEndAngle; angle >= sweepStartAngle; angle--) {
    moveTo(angle);
    delay(stepDelayMs);
  }
}
