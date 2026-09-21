#include <ESP32Servo.h>
#include <math.h>
// jmf277: Extra credit for the post-class task: move the servo with gradual acceleration and
// jmf277: deceleration instead of at a constant speed.
// jmf277: Build and upload it with the PlatformIO environment `servo_motor_ec` (see platformio.ini).

// jmf277: The servo's orange signal wire is on Feather A0 (GPIO 26); red and brown go to the breadboard
// jmf277: power module's 5 V rail (fed by a 9 V battery), as the handout's Figure 3 shows, and the module's - rail
// jmf277: is also wired to Feather GND so the signal and the servo share a ground.
Servo myServo;
const int servoPin = A0;

// jmf277: Same pulse-width range as the other servo sketches: 500 us = 0 degrees, 2500 us = 180 degrees.
const int minPulseWidth = 500;
const int maxPulseWidth = 2500;

// jmf277: One sweep goes from startAngle to endAngle in sweepTimeMs, then back. The command is
// jmf277: refreshed every updateIntervalMs, and the servo rests at each end for pauseMs.
const float startAngle = 0.0f;
const float endAngle = 180.0f;
const unsigned long sweepTimeMs = 2000;
const unsigned long updateIntervalMs = 10;
const unsigned long pauseMs = 500;

// jmf277: Two motion profiles. Both return the fraction of the sweep completed (0 to 1) for a
// jmf277: fraction of the sweep time p (0 to 1); their derivative is the speed.
// jmf277:   PROFILE 1, trapezoid: constant acceleration for the first third, constant speed for the
// jmf277:              middle third, constant deceleration for the last third (peak speed 1.5x average).
// jmf277:   PROFILE 2, cosine S-curve: the speed follows half a sine wave, so it starts at zero, peaks
// jmf277:              in the middle at (pi/2)x average, and returns to zero with no sudden jumps.
const int PROFILE = 2;

float trapezoidPosition(float p) {
  if (p < 1.0f / 3.0f) return 2.25f * p * p;                    // accelerating
  if (p < 2.0f / 3.0f) return 0.25f + 1.5f * (p - 1.0f / 3.0f); // cruising
  return 1.0f - 2.25f * (1.0f - p) * (1.0f - p);                // decelerating
}

float trapezoidSpeed(float p) {  // jmf277: derivative of trapezoidPosition, in sweeps per unit time
  if (p < 1.0f / 3.0f) return 4.5f * p;
  if (p < 2.0f / 3.0f) return 1.5f;
  return 4.5f * (1.0f - p);
}

float cosinePosition(float p) { return 0.5f - 0.5f * cosf(PI * p); }
float cosineSpeed(float p)    { return 0.5f * PI * sinf(PI * p); }

float profilePosition(float p) { return (PROFILE == 1) ? trapezoidPosition(p) : cosinePosition(p); }
float profileSpeed(float p)    { return (PROFILE == 1) ? trapezoidSpeed(p)    : cosineSpeed(p); }

// jmf277: Convert a fractional angle to a pulse width without rounding to whole degrees first.
int angleToMicroseconds(float angle) {
  return (int)roundf(minPulseWidth + (maxPulseWidth - minPulseWidth) * (angle / 180.0f));
}

// jmf277: Move from `from` to `to` over sweepTimeMs following the selected profile. Timing uses
// jmf277: millis(), so the sweep takes the same time no matter how long each loop iteration runs.
void sweep(float from, float to) {
  const unsigned long t0 = millis();
  unsigned long lastReport = 0;
  while (true) {
    unsigned long elapsed = millis() - t0;
    float p = fminf(1.0f, (float)elapsed / (float)sweepTimeMs);
    float angle = from + (to - from) * profilePosition(p);
    myServo.writeMicroseconds(angleToMicroseconds(angle));

    if (elapsed - lastReport >= 100 || elapsed == 0) {  // jmf277: report ten times per second
      float degPerSec = (p >= 1.0f) ? 0.0f : fabsf(to - from) * profileSpeed(p) * 1000.0f / (float)sweepTimeMs;
      Serial.printf("  t = %4lu ms  angle = %6.1f deg  speed = %5.1f deg/s\n", elapsed, angle, degPerSec);
      lastReport = elapsed;
    }
    if (p >= 1.0f) break;
    delay(updateIntervalMs);
  }
  myServo.writeMicroseconds(angleToMicroseconds(to)); // jmf277: land exactly on the target
}

void setup() {
  Serial.begin(115200);          // jmf277: match monitor_speed in platformio.ini
  myServo.setPeriodHertz(50);    // jmf277: standard 50 Hz servo pulse, set before attach()
  myServo.attach(servoPin, minPulseWidth, maxPulseWidth);
  // jmf277: The library's default 10-bit PWM timer only resolves the pulse width in steps of about
  // jmf277: 19.5 us (about 1.8 degrees), which would turn a smooth ramp into visible jerks. A 16-bit
  // jmf277: timer resolves about 0.3 us (about 0.03 degrees). In ESP32Servo 3.2.1, attach() resets the
  // jmf277: width to the 10-bit default, so setTimerWidth() has to come AFTER attach(): on an attached
  // jmf277: servo it re-attaches the pin at the new resolution, and the next write sets the pulse width.
  myServo.setTimerWidth(16);
  myServo.writeMicroseconds(angleToMicroseconds(startAngle));
  delay(pauseMs);
  Serial.printf("Servo Motor EC: profile %d (%s), %.0f-%.0f deg in %lu ms each way, %d-bit timer\n",
                PROFILE, (PROFILE == 1) ? "trapezoid" : "cosine S-curve", startAngle, endAngle, sweepTimeMs,
                myServo.readTimerWidth());
}

void loop() {
  Serial.println("Sweeping up: accelerate, then decelerate into the end stop");
  sweep(startAngle, endAngle);
  delay(pauseMs);
  Serial.println("Sweeping down: accelerate, then decelerate into the start");
  sweep(endAngle, startAngle);
  delay(pauseMs);
}
