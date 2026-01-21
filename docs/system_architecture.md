# System Architecture

This project implements a headless Battery Management System (BMS)
using embedded firmware and message-based communication.

## Overview

The system consists of three main components:

1. Embedded firmware running on ESP8266
2. MQTT broker for message transport
3. Linux-based command-line subscriber

## Data Flow

Battery-related signals are processed by the firmware and published
periodically over MQTT.

The backend subscriber receives these messages and outputs them to
the terminal or logs them for analysis.

## Design Principles

- Headless operation (no UI)
- Decoupled communication using MQTT
- Interrupt-driven firmware logic
- Linux-friendly tooling

## Extensibility

The architecture allows additional processing layers (analytics,
visualization, or ML) to be added without modifying the firmware.
