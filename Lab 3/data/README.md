# Lab 3 measurements

These CSV files contain all sensor readings from the final serial captures for the three exercises on September 12, 2026. The [lab report](../playing_with_sensors.md#experimental-results) explains the circuits, programs, results, and separate demonstration recordings.

| Dataset | Rows, excluding the header | Contents |
|---|---:|---|
| [potentiometer-readings.csv](potentiometer-readings.csv) | 1,251 | Raw potentiometer sweep and final held setting. |
| [voltage-readings.csv](voltage-readings.csv) | 712 | Potentiometer sweep with the raw ADC code and the program's printed voltage estimate. |
| [touch-readings.csv](touch-readings.csv) | 4,154 | Digital touch states and printed messages, including four complete touch/release cycles. |

## Column definitions

| Column | Meaning |
|---|---|
| `elapsed_seconds` in the raw-potentiometer file; `elapsed_host_seconds` in the other files | Seconds elapsed on the computer receiving the serial output, relative to the start of that capture. |
| `adc` | Integer 12-bit ADC code, from 0 through 4095. |
| `estimated_voltage` | The value printed by `voltage.cpp`, in volts, using nominal `adc × 3.3 / 4095` scaling and three decimal places. |
| `touch_state` | `1` for touch detected; `0` for no touch. |
| `serial_message` | The corresponding `Touch detected!` or `No touch detected...` text. |

The timestamps describe **host receipt**, including serial buffering. Adjacent rows can share a timestamp, and the recordings include idle time before and after the physical actions. Use the row order to follow the captured sequence; these timestamps do not establish exact device sampling intervals, touch durations, or response latency.

The voltage values are the program's nominal estimates, not calibrated measurements. Touch messages record the module input state; the [touch demonstration](../videos/touch-led-demo.mp4) also shows the physical LED response. The [potentiometer demonstration](../videos/potentiometer-voltage-demo.mp4) is a separate recording and ends at a different intermediate knob setting from the voltage CSV.
