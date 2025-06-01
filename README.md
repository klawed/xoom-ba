## **Project Overview**

**ESP32/XOOM + 4096 OLED Access Point with Sensor and Network Diagnostics**

This project uses an ESP32 (or XOOM board with ESP32 core), equipped with a 4096-pixel OLED display (likely a 128x32 or 128x64 screen), to operate as a WiFi access point. It continuously scans for available WiFi and Bluetooth networks and reads all connected sensors, displaying detailed diagnostics on the OLED. Users can connect to the AP and view this information through a simple web interface as well.

---

## **Key Features:**

### 1. **Access Point Functionality**
- **ESP32 sets up its own WiFi access point**
    - Custom SSID (e.g., "DiagAP")
    - Captive landing page with web UI

### 2. **WiFi and Bluetooth Scanning**
- **Continuously scans for:**
    - Nearby WiFi networks: SSID, RSSI, security type, channel, MAC
    - Bluetooth devices: MAC, device name, RSSI, type (BLE/classic)
- **Displays the N most recent (or strongest) networks/devices**

### 3. **Sensor Diagnostics**
- **All connected I2C/SPI/analog sensors polled**
    - Example sensors: DHT22/BME280 (temperature, humidity, pressure), light, accelerometer, etc.
- **Readings updated in real-time**

### 4. **OLED Display Output**
- **Scrolling/rotating displays for:**
    - Sensor values
    - Network lists (page per type if needed)
    - AP IP and connection info
    - Errors/warnings (e.g., sensor disconnects)

### 5. **Web UI / API**
- **Landing page upon AP connect:**
    - Live display of OLED info
    - JSON REST API for automation/scripts
    - Optional webserver for OTA updates

---

## **Hardware Components**

- **ESP32 Dev Board** (WiFi+Bluetooth capable)
    - Or a XOOM with ESP32 core
- **OLED Display (4096 pixels)**
    - e.g., 128x32 or 128x64 via I2C/SPI
- **Sensors** (your choice: environmental, motion, etc.)
- **Power supply**

---

## **Software Flow Outline**

1. **System Boot**
    - Initializes OLED
    - Enumerates sensors (auto-detect if possible)
    - Starts WiFi AP (with captive portal/web server)
    - Starts periodic WiFi/Bluetooth scans

2. **Main Loop**
    - Updates sensor readings
    - Updates network/Bluetooth lists
    - Formats text/pages for OLED cycling every few seconds
    - Handles web server/api requests

3. **Display Management**
    - OLED pages: sensors → WiFi → Bluetooth → AP info → repeat
    - Auto-scroll/rotate or advance via GPIO button

4. **Web Interface**
    - Live stats (mirroring OLED)
    - Config/settings page (AP name, scan interval, etc.)

---

## **Example OLED Displays**

```
[ Sensors ]
Temp: 24.3°C  Hum: 50%
Light: 220lx  Press: 1013hPa

[ WiFi ]
1. HomeNet  ( -43dBm )
2. Cafe_24  ( -68dBm )
3. AP_FREE  ( -78dBm )
...

[ Bluetooth ]
1. SmartWatch  F3:28:D0  -62dBm
2. JBL Speaker F8:1E:4A  -70dBm
...

[ AP Info ]
SSID: DiagAP  
IP: 192.168.4undefined
