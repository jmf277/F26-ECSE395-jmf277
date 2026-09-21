#include <ESP32Servo.h>
// jmf277: Completed from the course's Servo Motor Random.cpp starter for the second half of the post-class task.
// jmf277: Build and upload it with the PlatformIO environment `servo_motor_random` (see platformio.ini).
// jmf277: The ESP32Servo library is installed automatically by the lib_deps line in platformio.ini.

// jmf277: The servo's orange signal wire is on Feather A0 (GPIO 26); red and brown go to the breadboard
// jmf277: power module's 5 V rail (fed by a 9 V battery), as the handout's Figure 3 shows, and the module's - rail
// jmf277: is also wired to Feather GND so the signal and the servo share a ground.
Servo myServo;
const int servoPin = A0; // jmf277: the starter used the placeholder 0

// variable for random angle
int randomAngle;

// Variable for pulse width
int pulseWidth;

// jmf277: variable for the random pause after each move, in milliseconds
unsigned long randomDelayMs;

// Define the minimum and maximum pulse widths for the servo
const int minPulseWidth = 500; // 0.5 ms
const int maxPulseWidth = 2500; // 2.5 ms

// jmf277: Limits for the random pause, so the moves are sometimes quick and sometimes slow.
const unsigned long minDelayMs = 250;
const unsigned long maxDelayMs = 1500;

void setup() {
  Serial.begin(115200); // jmf277: match monitor_speed in platformio.ini so each move is reported

  // Set the PWM frequency for the servo
  myServo.setPeriodHertz(50); // Standard 50Hz servo (jmf277: set before attach(), as in the ESP32Servo examples)

  // Attach the servo to the specified pin and set its pulse width range
  myServo.attach(servoPin, minPulseWidth, maxPulseWidth);

  // jmf277: No randomSeed() call is needed: on the ESP32, random() reads the chip's hardware
  // jmf277: random-number generator unless randomSeed() has been called, so every run differs.
  Serial.println("Servo Motor Random: random angle between 0 and 180 degrees, random pause between moves");
}

void loop() {
    //  --- SECTION 1: Make a Random Angle Between 0 to 180 ---
    // jmf277: random(A, B) returns a value from A up to B - 1, so random(0, 181) can produce every
    // jmf277: whole angle from 0 through 180 inclusive.
    randomAngle = random(0, 181);

    // ---SECTION 2: Map Pulse Width with Angle
    // jmf277: As in Servo Motor.cpp, map() scales the angle proportionally: 0 degrees becomes
    // jmf277: minPulseWidth (500 us), 180 degrees becomes maxPulseWidth (2500 us), 90 degrees becomes 1500 us.
    // jmf277: map() takes five arguments: map(value, fromLow, fromHigh, toLow, toHigh).
    pulseWidth = map(randomAngle, 0, 180, minPulseWidth, maxPulseWidth);
    myServo.writeMicroseconds(pulseWidth); // writing pulse width to servo

    // jmf277: Pick a different pause every time instead of the starter's fixed 1000 ms.
    randomDelayMs = random(minDelayMs, maxDelayMs + 1);

    Serial.printf("Angle: %d  pulse: %d us  next move in %lu ms\n", randomAngle, pulseWidth, randomDelayMs);

    delay(randomDelayMs); // jmf277: wait for the servo to reach the angle and hold it there for the random time
}
