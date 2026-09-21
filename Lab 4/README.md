# Lab 4: Actuator Adventures

**Joe Falkenburg · `jmf277` · ECSE 395**

This lab uses an Adafruit Feather ESP32 V2 to drive two actuators from the SunFounder Universal Maker Sensor Kit: a TT motor (DC gear motor) through an L9110 motor-driver module, and an SG90 servo motor.

The [lab report](actuator_adventures.md) explains the setup, circuits, programs, parameter trials and their observed behavior, and the time report and reflection. The course's own instructions for the two tasks are in [Instructions/TT Motor.md](Instructions/TT%20Motor.md) and [Instructions/Servo Motor.md](Instructions/Servo%20Motor.md), and the [Feather pinout](photos/Adafruit%20ESP32%20GPIO%20Pinout.png) they refer to is in the `photos` folder, together with the updated wiring diagrams from the course template (September 17, 2026) and the two figures from the revised handout. The instruction files predate those diagrams and still power the motors from the Feather's `VBUS` pin; my circuits follow the handout's Figures 2 and 3 instead, with the kit's breadboard power module on a 9 V battery powering the motors and a jumper from the Feather's `GND` to the module's − rail as the common ground, as the report explains. The equipment used for this lab is stated in the report's [Equipment](actuator_adventures.md#equipment) section. The ESP32 and the SunFounder kit are CWRU-owned course equipment issued to me directly by the teaching team, and Dr. Fu approved my taking the kit home to finish the lab; equipment issued this way did not go through the check-out form.

## Programs

| Source | Environment | Task |
|---|---|---|
| [TT Motor.cpp](src/TT%20Motor.cpp) | `tt_motor` | Run the motor once; the `TRIAL` constant selects the starter, a changed `analogWrite()` value, swapped values, or a changed `delay()`. |
| [TT Motor Rotate.cpp](src/TT%20Motor%20Rotate.cpp) | `tt_motor_rotate` | Clockwise 5 s, stop 2 s, counterclockwise 5 s, stop 2 s, repeated. |
| [TT Motor EC.cpp](src/TT%20Motor%20EC.cpp) | `tt_motor_ec` | Extra credit: ramp the speed up and down continuously in both directions. |
| [Servo Motor.cpp](src/Servo%20Motor.cpp) | `servo_motor` | Sweep 0°–180° and back; the `TRIAL` constant selects the starter or one changed parameter (`minPulseWidth`, `maxPulseWidth`, `setPeriodHertz`, rotation range, `delay`). |
| [Servo Motor Random.cpp](src/Servo%20Motor%20Random.cpp) | `servo_motor_random` | Random angles between 0° and 180° with random pauses. |
| [Servo Motor EC.cpp](src/Servo%20Motor%20EC.cpp) | `servo_motor_ec` | Extra credit: gradual acceleration and deceleration with a trapezoid or cosine motion profile. |

Open this folder as a PlatformIO project. The [configuration](platformio.ini) pins `espressif32@7.1.0`, selects `adafruit_feather_esp32_v2` and the Arduino framework, sets a 115200-baud monitor, and lists the ESP32Servo library, which PlatformIO installs on the first build. Each program has its own environment that compiles only that file, so pick the environment for the program you want to run; see the report's [upload process](actuator_adventures.md#upload-process).

## Running a program

1. With USB disconnected and the power module switched off, build the circuit from the report's wiring tables: the L9110 module's `A-1A` and `A-1B` to Feather `A0` and `A1`, its `VCC` and `GND` to the module's 3.3 V rail (+ and −), a jumper from Feather `GND` to the − rail, and the TT motor's pins in the Motor A terminals; or the servo's orange wire to `A0`, its red and brown wires to the module's 5 V rail, and the same `GND` jumper. Then switch the module on.
2. Select the environment in the VS Code status bar or PlatformIO's Project Tasks, then run **Build**.
3. Connect the USB-C data cable, close any serial monitor using the board, and run **Upload**.
4. Open **Monitor** at **115200 baud** to see the program's messages (`Clockwise`, `Angle: 45  pulse: 1000 us`, and so on). Use **Control+C** to stop the monitor before another upload.

## Photos and demonstrations

| File | What it shows |
|---|---|
| [photos/tt-motor-circuit.jpg](photos/tt-motor-circuit.jpg) | The assembled TT motor circuit. |
| [photos/servo-circuit.jpg](photos/servo-circuit.jpg) | The assembled servo circuit. |
| [videos/tt-motor-rotate-demo.mp4](videos/tt-motor-rotate-demo.mp4) | `TT Motor Rotate.cpp` running: clockwise, stop, counterclockwise, stop. |
| [videos/servo-random-demo.mp4](videos/servo-random-demo.mp4) | `Servo Motor Random.cpp` running: random angles and pauses. |
| [videos/tt-motor-ec-demo.mp4](videos/tt-motor-ec-demo.mp4) | `TT Motor EC.cpp` running: the speed ramping up and down in both directions. |
| [videos/servo-ec-demo.mp4](videos/servo-ec-demo.mp4) | `Servo Motor EC.cpp` running: the sweep accelerating and decelerating. |

Reference: [SunFounder Lesson 34: TT Motor (ESP32)](https://docs.sunfounder.com/projects/umsk/en/latest/03_esp32/esp32_lesson34_motor.html) and [Lesson 33: Servo Motor (ESP32)](https://docs.sunfounder.com/projects/umsk/en/latest/03_esp32/esp32_lesson33_servo.html).
