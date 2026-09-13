# Lab 3: Playing with Sensors

**Joe Falkenburg · `jmf277` · ECSE 395**

This lab uses an Adafruit Feather ESP32 V2 to read a potentiometer, convert its analog readings to estimated voltage, and use a touch sensor to control the board's red LED.

The [lab report](playing_with_sensors.md) explains the setup, circuits, program implementation, measured results, and time reporting/reflection. It also gives the steps for selecting, uploading, and running each program.

## Programs

| Source | Exercise |
|---|---|
| [potentiometer.cpp](src/potentiometer.cpp) | Read and print the potentiometer's raw 12-bit ADC value. |
| [voltage.cpp](src/voltage.cpp) | Convert the stored ADC reading with a floating-point function and print the voltage estimate. |
| [touch.cpp](src/touch.cpp) | Print touch/release messages and turn the onboard red LED on/off with contact. |

Open this folder as a PlatformIO project. The [configuration](platformio.ini) selects `adafruit_feather_esp32_v2`, the Arduino framework, and a 115200-baud serial monitor. **`touch.cpp` is currently active**; the other sketches, including the unused `main.cpp` starter, are enclosed in block comments so only one `setup()` and `loop()` are compiled. Follow the report's [program-selection and upload instructions](playing_with_sensors.md#selecting-and-running-a-program) to change exercises.

## Running an exercise

1. With USB disconnected, connect the selected module's `VCC` to Feather `3V`, `GND` to `GND`, and signal (`OUT` for the potentiometer or `IO` for touch) to `A1`. Use one sensor at a time and follow the report's wiring table.
2. Activate the desired sketch using the report's block-comment selection steps, then run **Build** for `adafruit_feather_esp32_v2` in PlatformIO's Project Tasks.
3. Connect the USB-C data cable, close any existing serial monitor using the board, and run **Upload**.
4. Open **Monitor** at **115200 baud**. Turn the knob to observe raw/voltage changes, or touch and release the pad to observe the messages and red LED response. Use **Control+C** to stop the monitor before another upload.

## Demonstrations and measurements

| Demonstration | What it shows |
|---|---|
| [Potentiometer and voltage](videos/potentiometer-voltage-demo.mp4) · about 33 seconds | The assembled circuit, knob movement, and readable changing voltage estimates. |
| [Touch sensor and LED](videos/touch-led-demo.mp4) · about 23 seconds | Touch/release, the serial messages, and the Feather's red LED turning on/off. |

The [measurement appendix](data/README.md) contains the three final serial datasets and their column definitions. The report distinguishes these captures from the separate demonstration recordings.
