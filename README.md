# BMS_SoC
# ESP8266 Firmware – Headless Battery Management System

This directory contains the embedded firmware for a headless Battery
Management System (BMS) prototype built using ESP8266.

The firmware is responsible for sensor acquisition, safety monitoring,
and publishing battery status over MQTT. The system is intentionally
designed without any UI or mobile application.

---

## Responsibilities

- Read battery-related signals
- Handle interrupt-driven pulse counting
- Perform periodic tasks using timers
- Monitor temperature and trigger safety alerts
- Publish State of Charge (SoC) over MQTT
- Output structured logs via Serial for debugging

---

## Hardware Used

- ESP8266
- Temperature sensor (OneWire / DallasTemperature)
- LED and buzzer for alerts
- External interrupt source (pulse input)

---

## Key Design Choices

- **Headless operation**: No display or mobile application
- **Interrupt-based logic**: Efficient event counting
- **Timer-based scheduling**: Periodic SoC evaluation
- **MQTT messaging**: Decoupled data transport
- **Serial logging**: Primary debugging and observability mechanism

---

## Build & Flash

- Arduino IDE
- ESP8266 board package installed
- Select correct COM port and board

---

## Configuration

Network and MQTT credentials are stored in a local configuration file
(`config.h`) which is intentionally excluded from version control for
security reasons.

A sample configuration file (`config.h.example`) is provided.

---

## Debugging

- Use Arduino Serial Monitor at **115200 baud**
- Logs include sensor readings, SoC updates, and alert status

---

## Notes

This firmware focuses on reliability and clarity rather than advanced
analytics. It is designed to integrate cleanly with Linux-based systems
via MQTT and can be extended in the future if needed.

