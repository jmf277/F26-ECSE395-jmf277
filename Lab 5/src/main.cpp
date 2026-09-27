#include <Arduino.h> // jmf277: Arduino functions for the ESP32: pinMode, digitalRead, digitalWrite, millis, delay, Serial, neopixelWrite

// jmf277: Lab 5 sensor-actuator integration: a one-pad touch command controller for a TT motor.
// jmf277: The sensor is the SunFounder touch module and the actuator is the TT motor driven through the L9110 module.
// jmf277: 1 tap runs the motor forward for 3 s, 2 taps run it in reverse for 3 s, and 3 taps swing it forward
// jmf277: and back until the pad is touched again, for at most 20 s. A touch while a command is running stops it.
// jmf277: The Feather's built-in NeoPixel shows each tap and the command that is running.

// jmf277: Wiring (integration_exploration.md has the full breadboard table):
// jmf277:   Feather A0 (GPIO 26) to L9110 A-1A, and Feather A1 (GPIO 25) to L9110 A-1B
// jmf277:   Touch module IO to Feather A2 (GPIO 34), VCC to Feather 3V, GND to Feather GND
// jmf277:   L9110 VCC and GND to the breadboard power module's rail set to 3.3 V (9 V battery);
// jmf277:   that rail's ground is also wired to Feather GND, so every part shares one ground

// jmf277: Pin assignments. A0 and A1 can drive outputs; A2 is an input-only pin, which suits the touch signal.
const int MOTOR_A_1A = A0; // jmf277: GPIO 26 drives the driver's A-1A input
const int MOTOR_A_1B = A1; // jmf277: GPIO 25 drives the driver's A-1B input
const int TOUCH_PIN = A2;  // jmf277: GPIO 34 reads the touch module's IO output: HIGH while touched, LOW otherwise

// jmf277: Timing rules, all in milliseconds.
const unsigned long CALIBRATION_MS = 1000;  // jmf277: ignore the pad for 1 s at start-up; the touch chip needs about 0.5 s after power-up to calibrate
const unsigned long DEBOUNCE_MS = 30;       // jmf277: a change on the touch pin must last 30 ms before it is accepted, which filters out glitches
const unsigned long MAX_TAP_MS = 1000;      // jmf277: a touch released within 1 s is a tap; a longer hold is ignored
const unsigned long TAP_WINDOW_MS = 600;    // jmf277: after each tap, wait 0.6 s for another one before acting on the count
const unsigned long RUN_MS = 3000;          // jmf277: 1 tap and 2 taps run the motor for 3 s
const unsigned long SWING_MS = 1000;        // jmf277: each forward or reverse leg of the 3-tap mode lasts 1 s
const unsigned long PAUSE_MS = 500;         // jmf277: in the 3-tap mode, the motor stops for 0.5 s before each change of direction
const unsigned long SWING_LIMIT_MS = 20000; // jmf277: the 3-tap mode stops by itself after 20 s if the pad is not touched
const unsigned long RED_MIN_MS = 500;       // jmf277: a red warning stays lit for at least 0.5 s, unless a new touch starts sooner

// jmf277: NeoPixel colors. Each channel runs from 0 to 255; 40 keeps the light at about a sixth of full brightness.
const uint8_t LIGHT_LEVEL = 40; // jmf277: brightness used for every color
struct LightColor {             // jmf277: one color, stored as its red, green and blue levels
  uint8_t red;                  // jmf277: red level
  uint8_t green;                // jmf277: green level
  uint8_t blue;                 // jmf277: blue level
};
const LightColor LIGHT_OFF = {0, 0, 0};                                  // jmf277: off: waiting for taps
const LightColor LIGHT_WHITE = {LIGHT_LEVEL, LIGHT_LEVEL, LIGHT_LEVEL}; // jmf277: white: a finger is on the pad
const LightColor LIGHT_GREEN = {0, LIGHT_LEVEL, 0};                      // jmf277: green: running forward
const LightColor LIGHT_BLUE = {0, 0, LIGHT_LEVEL};                       // jmf277: blue: running in reverse
const LightColor LIGHT_PURPLE = {LIGHT_LEVEL, 0, LIGHT_LEVEL};           // jmf277: purple: the 3-tap back-and-forth
const LightColor LIGHT_RED = {LIGHT_LEVEL, 0, 0};                        // jmf277: red: stopped by a touch, or the touch was not a command

// jmf277: The three states the program can put the motor driver in.
enum MotorDirection { MOTOR_STOP, MOTOR_FORWARD, MOTOR_REVERSE };

// jmf277: The 3-tap mode repeats four steps: forward, pause, reverse, pause.
const MotorDirection SWING_STEPS[4] = {MOTOR_FORWARD, MOTOR_STOP, MOTOR_REVERSE, MOTOR_STOP}; // jmf277: what the motor does in each step
const unsigned long SWING_STEP_MS[4] = {SWING_MS, PAUSE_MS, SWING_MS, PAUSE_MS};             // jmf277: how long each step lasts
const char *const SWING_STEP_NAMES[4] = {"forward", "pause", "reverse", "pause"};             // jmf277: each step's name in the serial monitor

// jmf277: What the program is doing at any moment.
enum ProgramMode {
  WAITING_FOR_TAPS,   // jmf277: motor stopped; counting taps
  RUNNING_FORWARD,    // jmf277: the 1-tap command is running
  RUNNING_REVERSE,    // jmf277: the 2-tap command is running
  SWINGING,           // jmf277: the 3-tap command is running
  WAITING_FOR_RELEASE // jmf277: motor stopped; after a stopping touch, a long hold, or a touch at start-up, new taps count once the finger lifts
};
ProgramMode mode = WAITING_FOR_TAPS; // jmf277: the program starts out waiting for taps

bool touched = false;                 // jmf277: the pad's accepted state after debouncing (true means touched)
bool lastReading = false;             // jmf277: the raw pin reading from the previous pass through loop()
unsigned long readingChangedAt = 0;   // jmf277: when the raw reading last changed
bool touchPending = false;            // jmf277: true while a new touch is on the pin but has not yet held for 30 ms
unsigned long touchStartedAt = 0;     // jmf277: when the current touch began
unsigned long lastTapAt = 0;          // jmf277: when the most recent tap ended
int tapCount = 0;                     // jmf277: taps counted so far in the current group
unsigned long commandStartedAt = 0;   // jmf277: when the current motor command began
int swingStep = 0;                    // jmf277: which of the four 3-tap steps is running (0 to 3)
unsigned long swingStepStartedAt = 0; // jmf277: when that step began
bool redShowing = false;              // jmf277: whether the red warning is lit
unsigned long redStartedAt = 0;       // jmf277: when the red warning came on

// jmf277: Print one serial monitor line that starts with the time since start-up, for example "[   4.217 s] Tap 1".
void logEvent(const String &message) {
  Serial.printf("[%8.3f s] %s\n", millis() / 1000.0, message.c_str()); // jmf277: seconds to three decimals, then the message
}

// jmf277: Show a color on the Feather's built-in NeoPixel (GPIO 0).
void setLight(LightColor color) {
  neopixelWrite(PIN_NEOPIXEL, color.red, color.green, color.blue); // jmf277: ESP32 core function that sends the color to the NeoPixel
}

// jmf277: Turn on the red warning and note when it started.
void showRed(unsigned long now) {
  setLight(LIGHT_RED); // jmf277: light red
  redShowing = true;   // jmf277: mark the warning as lit
  redStartedAt = now;  // jmf277: note the time; it stays lit for at least RED_MIN_MS unless a new touch starts sooner
}

// jmf277: Set the driver's two Motor A inputs for the chosen direction. Only full drive is used, because on the
// jmf277: 3.3 V rail the motor did not start at half drive in Lab 4, so digitalWrite() drives each input fully high or low.
void setMotor(MotorDirection direction) {
  if (direction == MOTOR_FORWARD) {        // jmf277: forward: A-1A high, A-1B low
    digitalWrite(MOTOR_A_1B, LOW);         // jmf277: lower A-1B first, so the two inputs are never high together
    digitalWrite(MOTOR_A_1A, HIGH);        // jmf277: raise A-1A; the driver sends current through the motor one way
  } else if (direction == MOTOR_REVERSE) { // jmf277: reverse: A-1A low, A-1B high
    digitalWrite(MOTOR_A_1A, LOW);         // jmf277: lower A-1A first
    digitalWrite(MOTOR_A_1B, HIGH);        // jmf277: raise A-1B; the current flips, so the motor turns the other way
  } else {                                 // jmf277: stop: both inputs low
    digitalWrite(MOTOR_A_1A, LOW);         // jmf277: A-1A low
    digitalWrite(MOTOR_A_1B, LOW);         // jmf277: A-1B low; the driver pulls both motor terminals low, which brakes the motor
  }
}

// jmf277: A touch while a command is running, including the pauses of the 3-tap mode, stops the motor at once.
// jmf277: That touch is not counted as a tap.
void stopByTouch(unsigned long now) {
  setMotor(MOTOR_STOP);             // jmf277: stop the motor
  showRed(now);                     // jmf277: light red
  logEvent("Touch: motor stopped"); // jmf277: report the stop
  mode = WAITING_FOR_RELEASE;       // jmf277: ignore the pad until this touch ends
}

// jmf277: Begin one of the four 3-tap steps.
void startSwingStep(int step, unsigned long startTime) {
  swingStep = step;                                // jmf277: remember which step is running
  swingStepStartedAt = startTime;                  // jmf277: and when it began
  setMotor(SWING_STEPS[step]);                     // jmf277: forward, stop or reverse, from the step table
  logEvent(String("  ") + SWING_STEP_NAMES[step]); // jmf277: indented so the steps read as part of the 3-tap command
}

// jmf277: The tap window has closed, so act on the number of taps.
void startCommand(unsigned long now) {
  int count = tapCount;   // jmf277: keep the count for the checks below
  tapCount = 0;           // jmf277: the next group of taps starts from zero
  redShowing = false;     // jmf277: a new command replaces any red warning
  commandStartedAt = now; // jmf277: the command's running time is measured from here

  if (count == 1) {                                                        // jmf277: 1 tap
    logEvent("1 tap: forward for " + String(RUN_MS / 1000.0, 1) + " s");  // jmf277: report the command
    setMotor(MOTOR_FORWARD);                                               // jmf277: start turning forward
    setLight(LIGHT_GREEN);                                                 // jmf277: green while running forward
    mode = RUNNING_FORWARD;                                                // jmf277: loop() now times the run
  } else if (count == 2) {                                                 // jmf277: 2 taps
    logEvent("2 taps: reverse for " + String(RUN_MS / 1000.0, 1) + " s"); // jmf277: report the command
    setMotor(MOTOR_REVERSE);                                               // jmf277: start turning in reverse
    setLight(LIGHT_BLUE);                                                  // jmf277: blue while running in reverse
    mode = RUNNING_REVERSE;                                                // jmf277: loop() now times the run
  } else if (count == 3) {                                                 // jmf277: 3 taps
    logEvent("3 taps: back and forth until the pad is touched (" + String(SWING_LIMIT_MS / 1000.0, 1) + " s limit)"); // jmf277: report the command
    setLight(LIGHT_PURPLE);                                                // jmf277: purple for the whole 3-tap mode
    mode = SWINGING;                                                       // jmf277: loop() now steps through the cycle
    startSwingStep(0, now);                                                // jmf277: begin with the forward leg
  } else {                                                                 // jmf277: 4 or more taps
    logEvent(String(count) + " taps: not a command, ignored");            // jmf277: report that nothing will happen
    showRed(now);                                                          // jmf277: brief red flash; the motor stays stopped
  }
}

// jmf277: While the motor is stopped: count taps, reject long holds, and start a command once the tap window closes.
void handleTaps(unsigned long now, bool touchBegan, bool touchEnded) {
  if (touchBegan) {        // jmf277: a finger just landed on the pad
    touchStartedAt = now;  // jmf277: start timing the touch
    redShowing = false;    // jmf277: a new touch replaces any red warning
    setLight(LIGHT_WHITE); // jmf277: white while the finger is on the pad
  }

  if (touched && now - touchStartedAt > MAX_TAP_MS) { // jmf277: held longer than 1 s, so this touch is not a tap
    tapCount = 0;                                    // jmf277: discard any taps counted before it
    showRed(now);                                    // jmf277: light red
    logEvent("Held longer than " + String(MAX_TAP_MS / 1000.0, 1) + " s: ignored, tap count cleared"); // jmf277: report it
    mode = WAITING_FOR_RELEASE;                      // jmf277: wait for the finger to lift
    return;                                          // jmf277: nothing else to do on this pass
  }

  if (touchEnded) {                      // jmf277: the finger lifted within 1 s, so the touch was a tap
    tapCount++;                          // jmf277: count it
    lastTapAt = now;                     // jmf277: the tap window starts now
    setLight(LIGHT_OFF);                 // jmf277: white goes off, so each tap shows as one white flash
    logEvent("Tap " + String(tapCount)); // jmf277: report the running count
  }

  // jmf277: The group is complete when 0.6 s pass with no new touch. If a touch reached the pin just before the window
  // jmf277: closed, the program waits up to 30 ms for the debounce to decide whether that touch joins the group.
  if (!touched && !touchPending && tapCount > 0 && now - lastTapAt >= TAP_WINDOW_MS) {
    startCommand(now); // jmf277: act on the count
    return;            // jmf277: the command has already set the motor and the light
  }

  if (redShowing && !touched && now - redStartedAt >= RED_MIN_MS) { // jmf277: a red warning has been lit for at least 0.5 s
    redShowing = false;                                            // jmf277: clear it
    setLight(LIGHT_OFF);                                           // jmf277: light off, ready for taps
  }
}

// jmf277: During a 1-tap or 2-tap run: stop on a touch, or when the 3 s are up.
void handleRun(unsigned long now, bool touchBegan) {
  if (touchBegan) {   // jmf277: a touch during the run
    stopByTouch(now); // jmf277: stops it at once
    return;           // jmf277: nothing else to do on this pass
  }

  // jmf277: If a touch reached the pin in the last 30 ms, the run keeps going until that touch is accepted
  // jmf277: and stops it, or the pin drops again.
  if (!touchPending && now - commandStartedAt >= RUN_MS) {
    setMotor(MOTOR_STOP);                    // jmf277: stop the motor
    setLight(LIGHT_OFF);                     // jmf277: light off
    logEvent("Run finished: motor stopped"); // jmf277: report the stop
    mode = WAITING_FOR_TAPS;                 // jmf277: ready for the next taps
  }
}

// jmf277: During the 3-tap mode: stop on a touch or after 20 s; otherwise move to the next step when the current one ends.
void handleSwing(unsigned long now, bool touchBegan) {
  if (touchBegan) {   // jmf277: a touch at any point in the cycle
    stopByTouch(now); // jmf277: stops it at once
    return;           // jmf277: nothing else to do on this pass
  }

  if (!touchPending && now - commandStartedAt >= SWING_LIMIT_MS) {                     // jmf277: 20 s without a touch (a touch still being debounced finishes first)
    setMotor(MOTOR_STOP);                                                             // jmf277: stop the motor
    setLight(LIGHT_OFF);                                                              // jmf277: light off
    logEvent(String(SWING_LIMIT_MS / 1000.0, 1) + " s limit reached: motor stopped"); // jmf277: report the stop
    mode = WAITING_FOR_TAPS;                                                          // jmf277: ready for the next taps
    return;                                                                           // jmf277: nothing else to do on this pass
  }

  if (now - swingStepStartedAt >= SWING_STEP_MS[swingStep]) {                          // jmf277: the current step has run its full time
    startSwingStep((swingStep + 1) % 4, swingStepStartedAt + SWING_STEP_MS[swingStep]); // jmf277: next step, timed from when this one was due to end
  }
}

// jmf277: After a stopping touch, a long hold, or a touch at start-up: wait for the finger to lift before counting new taps.
void handleRelease(bool touchEnded) {
  if (touchEnded) {                           // jmf277: the finger lifted
    logEvent("Pad released: ready for taps"); // jmf277: report it
    mode = WAITING_FOR_TAPS;                  // jmf277: count taps again; a red light goes off once it has been lit 0.5 s, or turns white if a new touch starts sooner
  }
}

// jmf277: Runs once at start-up: set up the pins, print the command list, and keep the pad ignored for the first second.
void setup() {
  pinMode(MOTOR_A_1A, OUTPUT); // jmf277: the ESP32 drives A-1A
  pinMode(MOTOR_A_1B, OUTPUT); // jmf277: the ESP32 drives A-1B
  setMotor(MOTOR_STOP);        // jmf277: both driver inputs low before anything else, so the motor cannot start by itself
  pinMode(TOUCH_PIN, INPUT);   // jmf277: GPIO 34 has no internal pull resistor; the touch module drives the pin high and low itself
  setLight(LIGHT_OFF);         // jmf277: NeoPixel off

  Serial.begin(115200);                                                     // jmf277: same baud rate as monitor_speed in platformio.ini
  Serial.println();                                                         // jmf277: blank line to separate this run from the boot messages
  Serial.println("Touch-command motor");                                    // jmf277: program name
  Serial.printf("  1 tap:  forward for %.1f s (green)\n", RUN_MS / 1000.0); // jmf277: list the commands at the top of the monitor
  Serial.printf("  2 taps: reverse for %.1f s (blue)\n", RUN_MS / 1000.0);  // jmf277: second command
  Serial.printf("  3 taps: forward %.1f s, pause %.1f s, reverse %.1f s, pause %.1f s, repeated until a touch or %.0f s (purple)\n",
                SWING_MS / 1000.0, PAUSE_MS / 1000.0, SWING_MS / 1000.0, PAUSE_MS / 1000.0, SWING_LIMIT_MS / 1000.0); // jmf277: third command
  Serial.println("  A touch while a command runs stops it (red).");                            // jmf277: the stop rule
  Serial.printf("Starting: keep your finger off the pad for %.1f s\n", CALIBRATION_MS / 1000.0); // jmf277: warn before the wait

  delay(CALIBRATION_MS);                      // jmf277: the touch chip calibrates for about 0.5 s after it first gets power, when the USB cable is plugged in
  setLight(LIGHT_OFF);                        // jmf277: send "off" again, in case the NeoPixel misread the first color while its data pin (GPIO 0) was leaving its boot state
  touched = (digitalRead(TOUCH_PIN) == HIGH); // jmf277: take the pad's current state as the starting state, not as a new touch
  lastReading = touched;                      // jmf277: the debounce starts from that same state
  readingChangedAt = millis();                // jmf277: and from this moment
  if (touched) {                                                       // jmf277: a finger is already on the pad
    logEvent("Pad is being touched: lift your finger before tapping"); // jmf277: ask for it to be lifted
    mode = WAITING_FOR_RELEASE;                                        // jmf277: that touch will not be counted
  } else {                                                             // jmf277: the pad is clear
    logEvent("Ready: tap 1, 2 or 3 times");                            // jmf277: report that taps now count
  }
}

// jmf277: Runs over and over: read and debounce the pad, then let the handler for the current mode act.
void loop() {
  unsigned long now = millis(); // jmf277: one time value for this whole pass

  bool reading = (digitalRead(TOUCH_PIN) == HIGH); // jmf277: raw pad reading: true while touched
  if (reading != lastReading) {                    // jmf277: the raw reading changed
    lastReading = reading;                         // jmf277: remember the new value
    readingChangedAt = now;                        // jmf277: and restart the 30 ms stability timer
  }
  bool touchBegan = false;                                           // jmf277: true only on the pass where a touch is accepted
  bool touchEnded = false;                                           // jmf277: true only on the pass where a release is accepted
  if (reading != touched && now - readingChangedAt >= DEBOUNCE_MS) { // jmf277: a new state that has held for 30 ms
    touched = reading;                                               // jmf277: accept it
    touchBegan = touched;                                            // jmf277: a touch, if the pad is now touched
    touchEnded = !touched;                                           // jmf277: a release, if it is not
  }
  touchPending = reading && !touched; // jmf277: a touch is on the pin but has not held for 30 ms yet

  switch (mode) {                              // jmf277: hand this pass to the handler for the current mode
    case WAITING_FOR_TAPS:                     // jmf277: motor stopped
      handleTaps(now, touchBegan, touchEnded); // jmf277: count taps and start commands
      break;                                   // jmf277: done with this pass
    case RUNNING_FORWARD:                      // jmf277: 1-tap run
    case RUNNING_REVERSE:                      // jmf277: 2-tap run
      handleRun(now, touchBegan);              // jmf277: time the run and watch for a stopping touch
      break;                                   // jmf277: done with this pass
    case SWINGING:                             // jmf277: 3-tap mode
      handleSwing(now, touchBegan);            // jmf277: step through the cycle and watch for a stopping touch
      break;                                   // jmf277: done with this pass
    case WAITING_FOR_RELEASE:                  // jmf277: after a stopping touch, a long hold, or a touch at start-up
      handleRelease(touchEnded);               // jmf277: wait for the finger to lift
      break;                                   // jmf277: done with this pass
  }

  delay(1); // jmf277: check the pad about 1000 times a second; the pause also gives the ESP32's background tasks time to run
}
