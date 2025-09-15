# Firmware
This repo contains electonic files for IoT air quality monitoring system.

# 🖥️ IoT Air Quality Monitoring System – Software Stack

This document provides a detailed explanation of how the **software stack** of the IoT Air Quality Monitoring System operates.  
The stack is intentionally **modular** and is divided into **three core sections**:

---

## 1️⃣ Sensor Section

**Purpose**  
Collects real-time environmental data from all connected air-quality sensors.

**Operation**  
- Each sensor (PM10, PM2.5, NO₂, O₃, CO, SO₂, NH₃, CO₂, Temperature, Relative Humidity, etc.) is read at scheduled intervals.  
- The Raspberry Pi Pico polls the sensors using appropriate interfaces (I²C, UART, or analog).  
- Raw measurements are timestamped using the real-time clock (RTC) and passed to the **Data Logger Section**.

**Key Responsibilities**
- Sensor initialization and calibration.
- Error checking and sensor-fault detection.
- Formatting data into a common internal structure before hand-off.

---

## 2️⃣ Data Logger Section

**Purpose**  
Acts as the **data buffer and local historian**, ensuring no readings are lost when the network is unavailable.

**Internal Structure**  
The logger maintains **two distinct storage areas**:

1. **Backup Area**  
   - A rolling archive of *all* measurements.  
   - Provides a complete history for local diagnostics or later upload.

2. **Temporary Queue**  
   - Holds **only unsent measurements** that still need to be transmitted to the backend.

**Operational Flow**  
1. Receives sensor data and writes it to both the **Backup Area** and the **Temporary Queue**.  
2. Passes a copy of the queued data to the **Networking Section** for transmission.  
3. Waits for an **acknowledgement (ACK)** signal from the Networking Section.  
4. **Deletes** a reading from the Temporary Queue **only when** a positive ACK is received.  
   - If no ACK arrives (e.g., network outage), the data remains in the queue and is retried on the next transmission cycle.

**Benefits**
- Guarantees **no data loss** even with intermittent connectivity.
- Allows easy reconstruction of historical data.

---

## 3️⃣ Networking Section

**Purpose**  
Handles all outbound communication from the device to the cloud/backend server.

**Supported Interfaces**
- **GSM/Cellular** for direct Internet access when LoRa is not available.

**Operational Flow**
1. Reads the items in the **Temporary Queue**.
2. Packages the data (e.g., JSON or binary payload) and sends it to the backend API or gateway.
3. Waits for a **server acknowledgement**:
   - If **ACK received**, sends a command back to the **Data Logger Section** to remove the successfully transmitted record from the Temporary Queue.
   - If **no ACK**, leaves the data untouched and retries later.

**Resilience**
- Automatic retry with exponential back-off.
- Optional dual-channel mode: LoRa as primary, GSM as failover.

---

## 🔄 End-to-End Data Flow

