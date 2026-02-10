# ⛵ Pilote Automatique

Pilote Automatique is an Arduino-based autopilot TIPE project for a sailboat. It combines onboard sensors (GPS, IMU, wind sensor), rudder control, and a Python bridge for data logging. Feel free to read the MCOT (French), it explains some details about the whys of the project.

## What this project does

- Reads boat state from multiple modules: communication input, IMU heading, GPS position, rudder feedback, and apparent wind angle.
- Supports **manual mode** (rudder follows RC/command input) and **unmanned mode** (rudder command computed from a setpoint).
- Can steer using either:
  - **heading follow** (`heading_sp`), or
  - **apparent wind angle follow** (`awa_sp`).
- Logs experiment data to CSV via the Python app bridge.

## Hardware

- Runs on Arduino UNO Q (if you don't want to log data, a classic Arduino UNO can make it through i guess).
- Uses the Adafruit PWM servo shield to control the rudder servo via I2C.
- Adafruit GPS Breakout for the GPS (not needed for the control, thus nice to plot paths after an experiment).
- Adafruit IMU Fusion Breakout to measure the heading.
- Custom 3D printed Windvane (a simple low friction potentiometer put on the top of the mast).
- A classic radio controller (with a switch) and receiver use to manually control the sail and the rest of the boat if needed.

## Repository layout

```text
.
├── app.yaml                 # App metadata for the runtime
├── python/
│   └── main.py              # Python bridge + CSV logger
└── sketch/
    ├── sketch.ino           # Main Arduino control loop
    ├── CONFIG.h             # Tunable constants and feature flags
    ├── COM.*                # RC / communication input
    ├── IMU.*                # Heading source
    ├── GPS.*                # Position source
    ├── WINDSENSOR.*         # Apparent wind angle source
    ├── RUDDER.*             # Rudder actuation + feedback
    └── DATA.*               # Bridge call for data persistence
```

## Control logic (high-level)

At each loop iteration:

1. Update all modules (`COM`, `IMU`, `GPS`, `WINDSENSOR`, `RUDDER`).
2. Save telemetry (`timestamp, lat, lng, heading, rudder_angle, awa, filtered_awa, unmanned_status`).
3. Apply control:
   - if unmanned mode is active:
     - use heading control (`Kp * (heading_sp - heading)`), or
     - use AWA-follow control (`Kp * (computed_heading_sp - heading)`).
   - otherwise: pass through manual rudder command.

The loop frequency is configurable with `loop_frequency_hz` in `CONFIG.h`.

## Configuration

Most parameters live in `sketch/CONFIG.h`.

Common values to adjust:

- Hardware pins (`PIN_*` constants)
- PWM/ADC calibration values
- `Kp` proportional gain
- `loop_frequency_hz` (guarded between 20 and 50 Hz)
- Feature flags:
  - `full_unmanned_mode`
  - `awa_follow_mode`
- Setpoints:
  - `heading_sp`
  - `awa_sp`

## Data logging

The Python side (`python/main.py`) creates CSV logs in `/home/data` named like:

```text
data_YYYYMMDD_HH.csv
```

It also snapshots `sketch/CONFIG.h` into the top of each file so every experiment keeps its configuration context.

## Getting started

1. Open and review `sketch/CONFIG.h`.
2. Configure your board target (`fqbn`) in `sketch/sketch.yaml`.
3. Build and upload the sketch using your Arduino tooling.
4. Run the Python app runtime so `python/main.py` can receive bridge calls and log data.

## Notes

- `full_unmanned_mode = true` compiles a simplified loop focused on remote rudder commands.
- In normal mode (`false`), all sensors and logging are active.
- Start with conservative gain (`Kp`) and validate behavior in controlled conditions before open-water testing.


## License

No license file is currently provided in this repository.


Usefull links : 

-Arduino Uno Q User Manual (useful to understand how the Arduino Uno Q works)
https://docs.arduino.cc/tutorials/uno-q/user-manual/
-Arduino App CLI (to understand how an Arduino Uno Q app runs)
https:://github.com/arduino/arduino-app-cli 
