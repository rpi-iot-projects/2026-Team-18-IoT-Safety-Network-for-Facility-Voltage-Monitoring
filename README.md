# 2026-Team-18-IoT-Safety-Network-for-Facility-Voltage-Monitoring

**Authors:** Timothy Blyumin and Rayhan Khan

## Table of Contents
- [Overview](#overview)
- [Hardware Components](#hardware-components)
- [Software and Dependencies](#software-and-dependencies)
- [Usage](#usage)
- [Results and Demonstration](#results-and-demonstration)

## Overview
Traditional non-contact voltage diagnostics pose significant safety risks and lack system-wide data persistence. This project bridges the gap by transforming physical grid signals into stable digital telemetry at the hardware edge. 

**Capstone Integration:** This IoT pipeline utilizes a data acquisition stage developed for the FWNCVSD Capstone, including the MCP39F511A measurement IC and STM32 MCU to encode waveforms before wireless transmission.

## Hardware Components
- **ESP32 (Node 1 & 2)**: Acts as the wireless bridge to the MQTT network.
- **Raspberry Pi**: Operates as a central broker, routing packets to UI and storage layers.

## Software and Dependencies
- **Embedded**: C++ (Arduino Framework), PubSubClient, ArduinoJson.
- **Backend**: Python 3.x, Paho-MQTT, SQLite3.
- **Frontend**: HTML5, CSS3, Vanilla JavaScript, Chart.js.

## Usage
1. **Firmware**: Deploy the code in `/firmware/` to two ESP32 nodes (update `Configuration` for each).
2. **Bridge**: Run `python3 pi_backend/bridge.py` to start the circular buffer persistence.
3. **Dashboard**: Serve the `/webapp/` directory and access via the Pi's IP address (make sure to change IP address for all code files). 

## Results and Demonstration
The system successfully monitors independent telemetry streams (TX_01 and TX_02) with sub-second latency via WebSocket broadcasts. Data persistence is managed autonomously via a 500-record rolling window, protecting the gateway's storage from fatigue while ensuring 24/7 uptime.
