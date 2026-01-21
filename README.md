# Headless Battery Management System using ESP8266 and MQTT

This repository contains a headless Battery Management System (BMS)
prototype focused on embedded firmware reliability and Linux-friendly
data communication.

The project is intentionally designed without a mobile application,
web UI, or cloud dashboard. The emphasis is on clean embedded design,
observability, and modular system architecture.

---

## Architecture

ESP8266 Firmware → MQTT Broker → Linux-based CLI Subscriber

- Firmware publishes battery status over MQTT
- Serial logs are used for local debugging
- Backend tools consume data via command-line interfaces

---

## Features

- Interrupt-driven event counting
- Periodic task scheduling using timers
- Temperature monitoring with safety alerts
- MQTT-based data publishing
- Headless operation (no UI dependencies)
- Secure handling of credentials

---

## Project Structure

---

## Technology Stack

- C / Arduino (ESP8266)
- MQTT
- Python (CLI utilities)
- Linux development environment
- Git

---

## Why Headless?

The system is designed to operate without a user interface to simplify
debugging, improve reliability, and align with embedded and Linux-based
deployment environments.

This approach also enables easier automation and testing.

---

## Security Considerations

Sensitive configuration such as WiFi and MQTT credentials are excluded
from version control and provided via local configuration files.

---

## Scope and Limitations

This project focuses on embedded firmware behavior and reliable data
transport. Advanced analytics, visualization, or ML-based estimation
are intentionally out of scope and can be layered on top if required.

---

## How to Run

Refer to the README files inside each subdirectory for module-specific
build and execution instructions.

---

## Author

Nivedha

