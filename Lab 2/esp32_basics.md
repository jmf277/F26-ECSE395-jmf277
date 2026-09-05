# ECSE 395 Lab 2: ESP32 Basics

**Student:** Joe Falkenburg

## Purpose

This was my first assignment working with the ESP32. I programmed an Adafruit Feather ESP32 V2 to blink its onboard red LED and send a custom message to my Mac. I used Visual Studio Code with the PlatformIO IDE extension to edit, build, and upload the program, using the Arduino framework rather than the standalone Arduino IDE.

## Setup

| Item | Configuration |
|---|---|
| Board | Adafruit Feather ESP32 V2 |
| Project name | Blink Test |
| Development tools | Visual Studio Code and PlatformIO IDE |
| Framework | Arduino |
| Connection | USB-C cable between the board and my Mac |
| Onboard LED pin | GPIO 13 |
| Serial baud rate | 115200 |
| Serial Monitor speed | 115200 |
| Espressif 32 platform version used for testing | 7.1.0 |

The program is in [main.cpp](Blink%20Test/src/main.cpp), and the project settings are in [platformio.ini](Blink%20Test/platformio.ini).

## Procedure

1. Installed the PlatformIO IDE extension in Visual Studio Code.
2. Created the Blink Test project inside the repository's `Lab 2` folder, selecting the Adafruit Feather ESP32 V2 board and Arduino framework.
3. Connected the board to my Mac with a USB-C cable and allowed the USB accessory to connect. I checked the available serial ports and confirmed that the board appeared as `/dev/cu.usbserial-58900006671`.
4. Replaced PlatformIO's generated addition example in `src/main.cpp` with the completed blink program based on the supplied `Skeleton Code.md`.
5. Kept the supplied LED pin assignment of `13` and configured that pin as an output in `setup()`.
6. Enabled serial communication with `Serial.begin(115200)` and set `monitor_speed = 115200` in `platformio.ini` so the Serial Monitor used the same baud rate.
7. Added the custom message `Joe's ESP32 says hello!` and set the LED to turn on for 250 milliseconds and off for 1250 milliseconds. I updated the comments to describe these settings.
8. Used PlatformIO's Build task to compile the program. The build completed successfully.
9. Used Upload to send the program to the board. PlatformIO automatically detected the USB serial port, and the upload completed successfully.
10. Opened Monitor to check the custom message and watched the onboard LED blink.
11. Recorded a demonstration video for inclusion in the repository.
12. Marked the comments describing my code modifications with my CWRU ID, `jmf277`.

## Program Behavior

Each pass through `loop()` prints `Joe's ESP32 says hello!`, turns the LED on for 250 milliseconds, and then turns it off for 1250 milliseconds.

The two delays add up to 1500 milliseconds, so the program repeats approximately once every 1.5 seconds. The shorter on-time and longer off-time produce a brief flash followed by a longer pause.

## Results and Demonstration

Both Build and Upload finished with `SUCCESS`. The upload output also reported that the written data was verified.

After uploading, I observed the onboard LED blinking. The Serial Monitor displayed `Joe's ESP32 says hello!` repeatedly as readable text at 115200 baud. These observations confirmed that the program was running and communicating with my Mac.

The on/off durations listed above are the values set in the code; I did not independently measure the LED timing.

[Demonstration video](videos/esp32-blink-demo.mov)

The video shows the blinking LED and the custom message in the Serial Monitor.

## Time Reporting and Reflection

### 1. How long did it take you to complete this assignment?

About 2 hours.

### 2. What level of difficulty would you associate with this assignment?

- [x] Low
- [ ] Medium
- [ ] High

### 3. If you associated medium/high difficulty with this assignment, what aspect did you find the most difficult?

Not applicable.

### 4. How comfortable do you currently feel with the course content?

Comfortable.

### 5. Do you have any additional information or feedback you would like to share with the instructors?

No additional feedback.