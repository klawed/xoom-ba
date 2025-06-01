# Required Libraries for XOOM-BA

Install these libraries through the Arduino IDE Library Manager:

## Core Libraries

| Library | Author | Version | Purpose |
|---------|--------|---------|---------|
| Adafruit SSD1306 | Adafruit | >=2.5.0 | OLED display driver |
| Adafruit GFX Library | Adafruit | >=1.11.0 | Graphics primitives |
| DHT sensor library | Adafruit | >=1.4.0 | DHT22 temperature/humidity |
| Adafruit BME280 Library | Adafruit | >=2.2.0 | BME280 environmental sensor |
| ArduinoJson | Benoit Blanchon | >=6.19.0 | JSON parsing and generation |

## Network Libraries

| Library | Author | Version | Purpose |
|---------|--------|---------|---------|
| ESPAsyncWebServer | me-no-dev | >=1.2.3 | Asynchronous web server |
| AsyncTCP | me-no-dev | >=1.1.1 | TCP library for ESP32 |

## Built-in ESP32 Libraries (No Installation Required)

- WiFi.h - WiFi functionality
- WebServer.h - Basic web server
- DNSServer.h - DNS server for captive portal
- Wire.h - I2C communication
- BluetoothSerial.h - Classic Bluetooth
- BLEDevice.h - Bluetooth Low Energy
- BLEScan.h - BLE scanning
- BLEAdvertisedDevice.h - BLE device handling

## Installation Instructions

### Method 1: Library Manager (Recommended)
1. Open Arduino IDE
2. Go to Sketch → Include Library → Manage Libraries
3. Search for each library by name
4. Click Install for each required library
5. Wait for installation to complete

### Method 2: Manual Installation
If a library is not available in the Library Manager:

1. Download the library ZIP file from GitHub
2. Go to Sketch → Include Library → Add .ZIP Library
3. Select the downloaded ZIP file
4. Restart Arduino IDE

## Version Compatibility

| ESP32 Core Version | Recommended | Notes |
|-------------------|-------------|-------|
| 2.0.0+ | ✅ | Latest stable |
| 1.0.6 | ✅ | Stable, well-tested |
| < 1.0.6 | ❌ | Missing BLE features |

## Library Conflicts

### Common Issues
- **Multiple WiFi libraries**: Ensure only ESP32 WiFi.h is used
- **JSON library versions**: Use ArduinoJson v6.x, not v5.x
- **Bluetooth conflicts**: Don't include both Classic and BLE if not needed

### Resolution
If you experience compilation errors:
1. Remove conflicting libraries from Libraries folder
2. Clean build (delete build cache)
3. Reinstall required libraries only