# Lab 5: Integration Exploration

**Joe Falkenburg · `jmf277` · ECSE 395**

This lab integrates a sensor and an actuator from the SunFounder Universal Maker Sensor Kit on an Adafruit Feather ESP32 V2: the touch module commands the TT motor through the L9110 motor-driver module. One tap runs the motor forward for 3 seconds, two taps run it in reverse for 3 seconds, three taps swing it back and forth until the pad is touched again (for at most 20 seconds), and a touch while a command is running stops the motor.

The [lab report](integration_exploration.md) explains the behavior, the circuit and every wire, how the program works, how to build and upload it, the demonstrations, and the time report and reflection. Everything I used except my own USB-C data cable came in the course lab kit, as the report's [Equipment](integration_exploration.md#equipment) section describes.

## Program

| Source | What it does |
|---|---|
| [src/main.cpp](src/main.cpp) | Counts taps on the touch module and runs the TT motor through the L9110 driver to match, shows each tap and command on the Feather's NeoPixel, and logs every event to the serial monitor. |

Open this folder as a PlatformIO project. The [configuration](platformio.ini) selects the `adafruit_feather_esp32_v2` board, the Arduino framework, the `espressif32@7.1.0` platform and a 115200-baud serial monitor. There are no libraries to install.

## Running it

1. With the USB cable unplugged and the power module switched off, build the circuit from the report's [wiring table](integration_exploration.md#circuit).
2. Plug the Feather into the computer with a USB-C data cable, keeping fingers off the touch pad while it powers up. Close any serial monitor that is using the board, then click **Upload** in the PlatformIO status bar.
3. Open the serial monitor (115200 baud) and press the Feather's **RESET** button. The program prints its command list, then `Ready: tap 1, 2 or 3 times`.
4. Switch the power module on and tap the pad once, twice or three times. A touch while a command is running stops the motor.

## Photos and demonstrations

| File | What it shows |
|---|---|
| [photos/touch-motor-circuit.jpg](photos/touch-motor-circuit.jpg) | The assembled circuit. |
| [photos/touch-motor-wiring-diagram.png](photos/touch-motor-wiring-diagram.png) | The wiring diagram, numbered to match the report's wiring table. |
| [videos/demo-1-one-two-four-taps.mp4](videos/demo-1-one-two-four-taps.mp4) | One tap, two taps, one tap again, and four taps. |
| [videos/demo-2-run-stopped-then-three-taps.mp4](videos/demo-2-run-stopped-then-three-taps.mp4) | A touch stopping a forward run partway, then three taps starting the back-and-forth. |
| [videos/demo-3-long-hold.mp4](videos/demo-3-long-hold.mp4) | A hold longer than 1 second, which is ignored. |
| [videos/demo-4-three-taps-touch-stop.mp4](videos/demo-4-three-taps-touch-stop.mp4) | Three taps starting the back-and-forth, and a touch stopping it. |
