# Atmospheric Data Acquisition System

## 📖 Overview
This project implements a **microcontroller-based GPS logging system** for high-altitude balloon experiments.  
The system records **time, location, altitude, satellite information, and environmental data (temperature & pressure)**  
at a sampling rate of **1 Hz**. Data is logged to an **SD card** in structured CSV format for reliable post-flight analysis.

The system was designed as part of a **Capstone experiment at Grambling State University** and validated for flights up to **~100,000 ft**.

---

## 🖼️ System Visualization

1. **GPS Module Overview**  
   ![Adafruit GPS Module](https://tse4.mm.bing.net/th/id/OIP.2_MEmGJ8oTZ0p4630Y4KkQHaFs?r=0&pid=Api)  

2. **Micro SD Card Breakout Board**  
   ![Micro SD Breakout Board](https://tse3.mm.bing.net/th/id/OIP.Ppa8DsjzIZxnk2ZhYBy4dwHaFj?r=0&pid=Api)  

3. **GPS Logger Shield Example**  
   ![GPS Logger Shield](https://tse3.mm.bing.net/th/id/OIP.2AGCjGYJvR3_k_qjZahnuAHaFj?r=0&pid=Api)  

4. **Wiring Example for GPS Breakout**  
   ![GPS Wiring](https://tse4.mm.bing.net/th/id/OIP.X2ElFiJGaY8ctkKt9fPbxQHaFj?r=0&pid=Api)  

---

## ✨ Features
- **GPS Data Acquisition**
  - Latitude, longitude, altitude
  - Time, date, satellite count, fix quality
- **Environmental Data Acquisition**
  - Temperature (ADC pin A0, calibrated via linear regression)
  - Pressure (ADC pin A1, calibrated via linear regression)
- **Data Logging**
  - Structured CSV with headers for easy import into MATLAB/Excel/Python
  - Dynamic file naming: `GDAT0.csv`, `GDAT1.csv`, … to prevent overwrites
  - Automatic file rollover every ~60 samples (~1 minute at 1 Hz)
- **Robust Logging System**
  - Interrupt-driven GPS read ensures no missed sentences
  - Data flushed to SD card after every write to minimize data loss
  - SPI interface for high-speed SD card operations

---

## ⚙️ System Architecture

### Hardware
- **Arduino Mega 2560** (or compatible board with multiple UARTs)
- **Adafruit Ultimate GPS Module** (UART interface at 9600 baud)
- **SD Card Module** (SPI interface, CS pin = 10)
- **Temperature Sensor** (A0 input)
- **Pressure Sensor** (A1 input)
- Power supply suitable for balloon flights

### Software
- **Language**: Arduino C++
- **Libraries**:
  - `<SPI.h>` → SPI communication for SD card
  - `<SD.h>` → SD card file operations
  - `<Adafruit_GPS.h>` → GPS parsing and sentence handling

---

## 🔄 Program Flow

### Initialization (`setup()`)
1. Start serial communication (`Serial` for debugging, `Serial1` for GPS).
2. Configure GPS output to **1 Hz** update rate.
3. Initialize SD card and open/create `GDAT0.csv`.
4. Write descriptive and column headers.
5. Enable interrupt-driven GPS character reads.

### Main Loop (`loop()`)
1. **Check for new GPS sentence**:
   - If available, parse NMEA string using Adafruit library.
   - Extract time, date, fix status, satellite count, coordinates, and altitude.
2. **Read environment sensors**:
   - Temperature = `m * ADC(A0) + yIntercept`
   - Pressure = `0.0512 * ADC(A1) - 34.028`
3. **Assemble CSV string** with all variables.
4. **Write to file & flush**:
   - Print to Serial Monitor (for debugging).
   - Append line to CSV file on SD card.
   - Increment line counter.
   - If counter > 60 → roll over to new file.

---

## 📊 Example Output

**File: `GDAT0.csv`**
```csv
Capstone experiment at Grambling State University and the labels for the columns are as follows:
Time, Date, Fix, FixQuality, #sat, Longitude, Latitude, Altitude, Temperature, Pressure
12:35:07, 09/10/2025, 1, 2, 9, -93.2123, 32.5402, 150.5, 24.3, 101.2
12:35:08, 09/10/2025, 1, 2, 9, -93.2124, 32.5403, 150.8, 24.2, 101.2
```

---

## 🛠️ Functions
- **`loop()`**: Main program cycle. Reads GPS/sensor data, writes CSV.
- **`GPS_config()`**: Configures GPS update rate and disables unused NMEA sentences.
- **`useInterrupt(boolean v)`**: Enables or disables GPS interrupt routine.
- **`SIGNAL(TIMER0_COMPA_vect)`**: Interrupt Service Routine (ISR) reading characters from GPS.
- **`getPressure()`**: Converts ADC value (A1) to calibrated pressure reading.
- **`getTemperature()`**: Converts ADC value (A0) to calibrated temperature reading.
- **`newFile()`**: Creates a new CSV file (`GDAT#.csv`) when 60+ samples are logged.

---

## ⚠️ Known Limitations
- **Delay in loop**: `delay(1000)` enforces 1 Hz timing but blocks execution. A `millis()`-based scheduler is more efficient.
- **Headers in rollover files**: Currently commented out; new files may not include column headers unless re-enabled.
- **SD wear**: `flush()` on every write is safe but reduces SD card lifespan. Batch flushing is recommended.

---

## 🚀 Future Improvements
- Replace `delay()` with **millis()-based scheduling**.
- Add **units** in CSV headers (°C, kPa).
- Log **battery voltage** for diagnostics.
- Support higher-rate logging (5–10 Hz GPS).
- Implement **checksum verification** for GPS strings.

---

## 🌍 Applications
- High-altitude balloon flights (atmospheric profiling)
- Remote environmental monitoring
- GPS-based tracking for research
- Embedded systems education

---
