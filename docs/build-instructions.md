# XOOM-BA Build Instructions

## Quick Start

### Required Hardware
- ESP32 Development Board (DevKit v1 recommended)
- 128x64 OLED Display (SSD1306, I2C interface)
- DHT22 Temperature/Humidity Sensor
- BME280 Environmental Sensor (optional but recommended)
- Light Dependent Resistor (LDR)
- 10kΩ Resistor (for LDR voltage divider)
- 220Ω Resistor (for status LED, optional)
- Push Button (optional)
- Breadboard or PCB
- Jumper wires
- USB cable for programming

### Required Software
- Arduino IDE 1.8.19 or newer
- ESP32 Board Package
- Required Libraries (see Library Installation section)

## Detailed Build Process

### Step 1: Software Setup

#### Install Arduino IDE
1. Download Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Install and launch Arduino IDE
3. Go to File → Preferences
4. Add ESP32 board manager URL:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
5. Click OK

#### Install ESP32 Board Package
1. Go to Tools → Board → Boards Manager
2. Search for "ESP32"
3. Install "ESP32 by Espressif Systems"
4. Wait for installation to complete

#### Install Required Libraries
Install the following libraries through Library Manager (Sketch → Include Library → Manage Libraries):

| Library Name | Author | Purpose |
|--------------|--------|---------|
| Adafruit SSD1306 | Adafruit | OLED display driver |
| Adafruit GFX Library | Adafruit | Graphics functions |
| DHT sensor library | Adafruit | DHT22 sensor |
| Adafruit BME280 Library | Adafruit | BME280 sensor |
| ArduinoJson | Benoit Blanchon | JSON handling |
| ESPAsyncWebServer | me-no-dev | Web server |
| AsyncTCP | me-no-dev | TCP library for web server |

### Step 2: Hardware Assembly

#### Breadboard Layout

```
ESP32 DevKit v1 Pinout Reference:
                  ┌─────────────────┐
                  │      ESP32      │
              3V3 │ 1            38 │ GND
               EN │ 2            37 │ GPIO23
          GPIO36  │ 3            36 │ GPIO22 (SCL)
          GPIO39  │ 4            35 │ GPIO21 (SDA)
          GPIO34  │ 5            34 │ GPIO19
          GPIO35  │ 6            33 │ GPIO18
          GPIO32  │ 7            32 │ GPIO5
          GPIO33  │ 8            31 │ GPIO17
          GPIO25  │ 9            30 │ GPIO16
          GPIO26  │10            29 │ GPIO4  (DHT22)
          GPIO27  │11            28 │ GPIO0  (BOOT)
          GPIO14  │12            27 │ GPIO2  (LED)
          GPIO12  │13            26 │ GPIO15
              GND │14            25 │ GPIO13
          GPIO13  │15            24 │ GPIO12
              GND │16            23 │ GPIO14
              VIN │17            22 │ GPIO27
              3V3 │18            21 │ GPIO26
                  │19            20 │ GPIO25
                  └─────────────────┘
```

#### Detailed Wiring Instructions

**OLED Display (I2C):**
```
OLED VCC  → ESP32 3.3V
OLED GND  → ESP32 GND
OLED SDA  → ESP32 GPIO21
OLED SCL  → ESP32 GPIO22
```

**DHT22 Sensor:**
```
DHT22 Pin 1 (VCC)  → ESP32 3.3V
DHT22 Pin 2 (DATA) → ESP32 GPIO4
DHT22 Pin 3 (NC)   → Not connected
DHT22 Pin 4 (GND)  → ESP32 GND
```

**BME280 Sensor (I2C):**
```
BME280 VCC → ESP32 3.3V
BME280 GND → ESP32 GND
BME280 SDA → ESP32 GPIO21 (shared with OLED)
BME280 SCL → ESP32 GPIO22 (shared with OLED)
```

**Light Sensor (Voltage Divider):**
```
LDR Terminal 1 → ESP32 3.3V
LDR Terminal 2 → ESP32 GPIO34 + 10kΩ resistor
10kΩ Resistor  → ESP32 GND
```

**Optional Push Button:**
```
Button Terminal 1 → ESP32 GPIO0 (Boot button)
Button Terminal 2 → ESP32 GND
```

**Optional Status LED:**
```
LED Anode (+)  → ESP32 GPIO2
LED Cathode (-) → 220Ω resistor → ESP32 GND
```

### Step 3: Assembly Steps

1. **Prepare Breadboard**
   - Insert ESP32 development board into breadboard
   - Ensure USB connector is accessible for programming

2. **Power Rails**
   - Connect ESP32 3.3V to breadboard positive rail
   - Connect ESP32 GND to breadboard negative rail

3. **OLED Display**
   - Insert OLED module into breadboard
   - Connect using jumper wires as specified above
   - Double-check I2C connections (SDA/SCL)

4. **DHT22 Sensor**
   - Insert DHT22 into breadboard
   - Connect power and data lines
   - Ensure proper orientation (check pin numbering)

5. **BME280 Sensor**
   - Insert BME280 module into breadboard
   - Connect I2C lines (shared with OLED)
   - Verify I2C address (0x76 or 0x77)

6. **Light Sensor Circuit**
   - Insert LDR into breadboard
   - Add 10kΩ resistor to form voltage divider
   - Connect analog input to GPIO34

7. **Optional Components**
   - Add push button if desired for manual page switching
   - Add external LED for status indication

### Step 4: Software Upload

#### Configure Arduino IDE
1. Connect ESP32 to computer via USB
2. Select board: Tools → Board → ESP32 Arduino → ESP32 Dev Module
3. Select port: Tools → Port → (your ESP32 port)
4. Set upload speed: Tools → Upload Speed → 921600

#### Upload Code
1. Download the main code file: `src/xoom-ba.ino`
2. Open in Arduino IDE
3. Verify/compile the code (Ctrl+R)
4. Upload to ESP32 (Ctrl+U)
5. Monitor serial output (Ctrl+Shift+M)

### Step 5: Testing and Verification

#### Initial Boot Test
1. **Power On**
   - Connect USB cable
   - OLED should display "XOOM-BA Starting..."
   - Serial monitor should show initialization messages

2. **Display Test**
   - OLED should cycle through different pages
   - Check sensor readings appear
   - Verify page transitions every 5 seconds

3. **Sensor Verification**
   ```
   Expected Serial Output:
   BME280 sensor found
   Access Point Started
   AP IP address: 192.168.4.1
   System initialization complete
   Found X WiFi networks
   Found Y Bluetooth devices
   ```

#### Network Connectivity Test
1. **WiFi Access Point**
   - Look for "DiagAP" network on phone/laptop
   - Connect to network (no password required)
   - Browser should auto-redirect to captive portal

2. **Web Interface**
   - Navigate to 192.168.4.1 if no auto-redirect
   - Verify dashboard loads with live data
   - Check auto-refresh functionality

3. **API Testing**
   - Test individual endpoints:
     - `http://192.168.4.1/api/status`
     - `http://192.168.4.1/api/sensors`
     - `http://192.168.4.1/api/wifi`
     - `http://192.168.4.1/api/bluetooth`

#### Sensor Calibration
1. **Temperature/Humidity**
   - Compare readings with known good thermometer
   - Check for reasonable values (not NaN)

2. **Pressure (BME280)**
   - Should read approximately 1013 hPa at sea level
   - Adjust for local elevation if needed

3. **Light Sensor**
   - Cover sensor - reading should decrease
   - Shine light - reading should increase
   - Typical range: 0-1000 lux

## Troubleshooting

### Common Issues

#### OLED Display Not Working
- **Symptoms**: Blank screen, no startup message
- **Solutions**:
  - Check I2C connections (SDA/SCL)
  - Verify power connections (3.3V/GND)
  - Try different I2C address (0x3C or 0x3D)
  - Check if display is SSD1306 compatible

#### Sensor Reading Errors
- **DHT22 Issues**:
  - Check data pin connection (GPIO4)
  - Verify power supply (3.3V)
  - Replace sensor if consistently reading NaN

- **BME280 Issues**:
  - Verify I2C address in serial monitor
  - Check for I2C conflicts with OLED
  - Try different BME280 module

#### WiFi Access Point Problems
- **Can't Find Network**:
  - Check serial monitor for AP start messages
  - Verify ESP32 is not in STA mode
  - Reset ESP32 and retry

- **Can't Connect**:
  - Check if maximum connections reached
  - Verify device supports 2.4GHz WiFi
  - Try forgetting and reconnecting

#### Web Interface Issues
- **Page Won't Load**:
  - Verify IP address (192.168.4.1)
  - Check if connected to DiagAP network
  - Clear browser cache

- **No Live Data**:
  - Check browser console for JavaScript errors
  - Verify API endpoints respond
  - Check if sensors are reading properly

### Debug Techniques

#### Serial Monitor Debug
```cpp
// Add debug prints in code:
Serial.println("Debug: Entering function X");
Serial.printf("Sensor value: %.2f\n", value);
```

#### LED Status Codes
- **Solid ON**: Boot/initialization
- **Solid OFF**: Normal operation
- **Blinking**: Error condition

#### Factory Reset
To reset to defaults:
1. Hold GPIO0 button during power-on
2. Or reflash firmware completely
3. Check for corrupted flash memory

## Advanced Configuration

### Custom Settings
Modify these constants in the code for customization:

```cpp
// Network Configuration
#define AP_SSID "YourCustomAP"
#define AP_PASSWORD "YourPassword"  // Set password for security
#define AP_CHANNEL 6               // Change channel to avoid interference

// Timing Configuration
#define DISPLAY_UPDATE_INTERVAL 3000   // 3 seconds between pages
#define SENSOR_UPDATE_INTERVAL 1000    // 1 second for sensors
#define WIFI_SCAN_INTERVAL 60000       // 1 minute for WiFi scan

// Hardware Configuration
#define DHT_PIN 5                      // Change DHT22 pin
#define LDR_PIN 35                     // Change LDR pin
#define BUTTON_PIN 0                   // Change button pin
```

### Performance Tuning
- **Memory Optimization**: Reduce array sizes for networks/devices
- **Power Saving**: Increase scan intervals, use deep sleep
- **Response Time**: Decrease update intervals (higher power usage)

### Security Enhancements
- **WiFi Password**: Set AP_PASSWORD to secure access point
- **Web Authentication**: Add basic auth to web interface
- **API Rate Limiting**: Implement request rate limiting

## Enclosure Design

### 3D Printed Case
- Design considerations for breadboard or PCB mounting
- Ventilation for sensors
- Display window and button access
- USB port accessibility

### Commercial Enclosure
- Recommended size: 100mm x 70mm x 30mm minimum
- Plastic or aluminum construction
- IP rating for outdoor use (IP54+)

### Mounting Options
- Velcro strips for temporary installation
- Screw mounts for permanent installation
- Magnetic mount for metal surfaces
- Suction cup for glass surfaces

This completes the comprehensive build instructions for the XOOM-BA project. The system should now be fully functional as a WiFi access point with network and sensor diagnostics capabilities.