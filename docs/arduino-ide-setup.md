# Arduino IDE Setup Guide for XOOM-BA

This guide will walk you through setting up the Arduino IDE specifically for the XOOM-BA project on ESP32.

## System Requirements

### Supported Operating Systems
- **Windows**: Windows 7 or newer (64-bit recommended)
- **macOS**: macOS 10.14 Mojave or newer
- **Linux**: Ubuntu 16.04 LTS or newer, most modern distributions

### Hardware Requirements
- **RAM**: Minimum 2GB, 4GB+ recommended
- **Storage**: At least 1GB free space
- **USB Port**: For ESP32 programming and serial communication

## Step 1: Download and Install Arduino IDE

### Option A: Arduino IDE 2.x (Recommended)

1. **Download Arduino IDE 2.x**
   - Visit [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
   - Click "Download" for your operating system
   - Choose the installer version (not the ZIP file)

2. **Install Arduino IDE**
   
   **Windows:**
   ```
   1. Run the downloaded .msi or .exe installer
   2. Follow the installation wizard
   3. Check "Install USB driver" when prompted
   4. Complete installation
   ```
   
   **macOS:**
   ```
   1. Open the downloaded .dmg file
   2. Drag Arduino IDE to Applications folder
   3. Launch from Applications
   4. Allow security permissions if prompted
   ```
   
   **Linux (Ubuntu/Debian):**
   ```bash
   # Download AppImage or use package manager
   sudo apt update
   sudo apt install arduino
   
   # Or download from website and:
   chmod +x arduino-ide_2.x.x_Linux_64bit.AppImage
   ./arduino-ide_2.x.x_Linux_64bit.AppImage
   ```

### Option B: Arduino IDE 1.8.x (Legacy)

If you prefer the classic interface:

1. **Download Arduino IDE 1.8.19**
   - Visit [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
   - Scroll down to "Legacy IDE (1.8.X)"
   - Download for your operating system

2. **Install following similar steps as above**

## Step 2: Install ESP32 Board Support

### Add ESP32 Board Manager URL

1. **Open Arduino IDE**
2. **Go to Preferences**
   - **Arduino IDE 2.x**: File → Preferences
   - **Arduino IDE 1.8.x**: File → Preferences (Windows/Linux) or Arduino → Preferences (macOS)

3. **Add Board Manager URL**
   - Find "Additional Board Manager URLs" field
   - Add this URL:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
   - If there are already URLs, separate with commas or use the button to add a new line

4. **Click OK** to save preferences

### Install ESP32 Board Package

1. **Open Boards Manager**
   - **Arduino IDE 2.x**: Click board dropdown → "Select other board and port..." → Search
   - **Arduino IDE 1.8.x**: Tools → Board → Boards Manager

2. **Search for ESP32**
   - Type "ESP32" in the search box
   - Look for "esp32 by Espressif Systems"

3. **Install ESP32 Package**
   - Click "Install" button
   - Choose latest stable version (2.0.0 or newer recommended)
   - Wait for download and installation (may take several minutes)

4. **Verify Installation**
   - Go to Tools → Board
   - You should see "ESP32 Arduino" section with multiple ESP32 boards

## Step 3: Install Required Libraries

### Using Library Manager (Recommended)

1. **Open Library Manager**
   - **Arduino IDE 2.x**: Click library icon in sidebar or Tools → Manage Libraries
   - **Arduino IDE 1.8.x**: Sketch → Include Library → Manage Libraries

2. **Install Core Libraries** (search and install each):

   **Display Libraries:**
   - `Adafruit SSD1306` by Adafruit (version 2.5.0+)
   - `Adafruit GFX Library` by Adafruit (version 1.11.0+)

   **Sensor Libraries:**
   - `DHT sensor library` by Adafruit (version 1.4.0+)
   - `Adafruit BME280 Library` by Adafruit (version 2.2.0+)
   - `Adafruit Unified Sensor` by Adafruit (auto-installed with above)

   **Network & JSON Libraries:**
   - `ArduinoJson` by Benoit Blanchon (version 6.19.0+)
   - `ESPAsyncWebServer` by me-no-dev (version 1.2.3+)
   - `AsyncTCP` by me-no-dev (version 1.1.1+)

3. **Installation Process for Each Library:**
   ```
   1. Search for library name
   2. Click on the correct library (check author)
   3. Select latest compatible version
   4. Click "Install"
   5. Install dependencies when prompted
   ```

### Manual Library Installation (If Needed)

If a library isn't available in the Library Manager:

1. **Download Library ZIP**
   - Go to the library's GitHub page
   - Click "Code" → "Download ZIP"

2. **Install ZIP Library**
   - Arduino IDE: Sketch → Include Library → Add .ZIP Library
   - Select the downloaded ZIP file
   - Library will be installed automatically

## Step 4: Configure ESP32 Development Environment

### Install USB Drivers

**Windows:**
- ESP32 DevKit boards usually use CP2102 or CH340 USB-to-serial chips
- Download drivers:
  - **CP2102**: [Silicon Labs CP210x Drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
  - **CH340**: [CH340 Drivers](http://www.wch.cn/downloads/CH341SER_EXE.html)
- Install the appropriate driver for your board

**macOS:**
- Usually works out of the box with modern macOS
- If needed, install CP2102 drivers from Silicon Labs

**Linux:**
- Add user to dialout group:
```bash
sudo usermod -a -G dialout $USER
# Log out and log back in for changes to take effect
```

### Board Configuration

1. **Connect ESP32 to Computer**
   - Use a good quality USB cable (data + power, not just power)
   - Connect to USB port on computer

2. **Select Board**
   - Go to Tools → Board → ESP32 Arduino
   - Select "ESP32 Dev Module" (works for most ESP32 boards)

3. **Select Port**
   - Go to Tools → Port
   - Select the port your ESP32 is connected to:
     - **Windows**: COM3, COM4, etc.
     - **macOS**: /dev/cu.usbserial-xxxx or /dev/cu.SLAB_USBtoUART
     - **Linux**: /dev/ttyUSB0, /dev/ttyACM0, etc.

4. **Configure Upload Settings**
   ```
   Tools → Board: "ESP32 Dev Module"
   Tools → Upload Speed: "921600"
   Tools → CPU Frequency: "240MHz (WiFi/BT)"
   Tools → Flash Frequency: "80MHz"
   Tools → Flash Mode: "QIO"
   Tools → Flash Size: "4MB (32Mb)"
   Tools → Partition Scheme: "Default 4MB with spiffs"
   Tools → Core Debug Level: "None"
   Tools → PSRAM: "Disabled"
   ```

## Step 5: Test Your Setup

### Basic ESP32 Test

1. **Create New Sketch**
   - File → New (or Ctrl+N)

2. **Enter Test Code**
   ```cpp
   void setup() {
     Serial.begin(115200);
     Serial.println("ESP32 Test - XOOM-BA Setup");
     pinMode(2, OUTPUT); // Built-in LED
   }
   
   void loop() {
     digitalWrite(2, HIGH);
     Serial.println("LED ON");
     delay(1000);
     digitalWrite(2, LOW);
     Serial.println("LED OFF");
     delay(1000);
   }
   ```

3. **Compile and Upload**
   - Click Verify (✓) to compile
   - Click Upload (→) to flash to ESP32
   - Open Serial Monitor (Tools → Serial Monitor)
   - Set baud rate to 115200
   - You should see output and LED blinking

### Library Test

1. **Test WiFi (Built-in)**
   ```cpp
   #include "WiFi.h"
   
   void setup() {
     Serial.begin(115200);
     WiFi.mode(WIFI_STA);
     WiFi.disconnect();
     delay(100);
     Serial.println("WiFi scan test");
   }
   
   void loop() {
     int n = WiFi.scanNetworks();
     Serial.printf("Found %d networks\n", n);
     for (int i = 0; i < n; i++) {
       Serial.printf("%d: %s (%d dBm)\n", i, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
     }
     delay(5000);
   }
   ```

2. **Upload and verify WiFi networks are detected**

## Step 6: Download XOOM-BA Code

### Clone Repository
```bash
git clone https://github.com/klawed/xoom-ba.git
cd xoom-ba
```

### Or Download ZIP
1. Go to [https://github.com/klawed/xoom-ba](https://github.com/klawed/xoom-ba)
2. Click "Code" → "Download ZIP"
3. Extract to your project folder

### Open XOOM-BA Code
1. Open Arduino IDE
2. File → Open
3. Navigate to `xoom-ba/src/xoom-ba.ino`
4. Click Open

## Troubleshooting

### Common Issues

#### ESP32 Not Detected
**Problem**: Port not showing up or "Failed to connect" error

**Solutions:**
```
1. Check USB cable (must support data, not just power)
2. Install proper USB drivers (CP2102/CH340)
3. Try different USB port
4. Hold BOOT button while connecting (some boards)
5. Check Device Manager (Windows) for unknown devices
```

#### Library Compilation Errors
**Problem**: Library not found or compilation errors

**Solutions:**
```
1. Verify library installation in Library Manager
2. Check library compatibility with ESP32
3. Update Arduino IDE and ESP32 core
4. Clear Arduino cache: Close IDE, delete temp folders
5. Reinstall problematic libraries
```

#### Upload Failures
**Problem**: "Failed to connect to ESP32" during upload

**Solutions:**
```
1. Hold BOOT button during upload
2. Reduce upload speed (Tools → Upload Speed → 115200)
3. Try different USB cable/port
4. Reset ESP32 before upload
5. Check power supply adequacy
```

#### Permission Errors (Linux)
**Problem**: Permission denied accessing serial port

**Solutions:**
```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER
# Or temporarily
sudo chmod 666 /dev/ttyUSB0
```

### Getting Help

- **Arduino Forum**: [forum.arduino.cc](https://forum.arduino.cc)
- **ESP32 Community**: [esp32.com](https://esp32.com)
- **XOOM-BA Issues**: [GitHub Issues](https://github.com/klawed/xoom-ba/issues)

## Advanced Configuration

### Increase Upload Speed
For faster uploads:
```
Tools → Upload Speed → 921600
# If unstable, try 460800 or 230400
```

### Enable Debug Output
For troubleshooting:
```
Tools → Core Debug Level → "Info" or "Debug"
```

### Custom Board Definitions
For specialized ESP32 variants:
```
Tools → Board → "ESP32 Dev Module"
# Then adjust specific settings as needed
```

## Next Steps

Once Arduino IDE is properly configured:

1. **Hardware Assembly**: Follow `docs/build-instructions.md`
2. **Component Wiring**: Use `docs/schematics.md`
3. **Code Upload**: Flash `src/xoom-ba.ino`
4. **Testing**: Verify all features work
5. **Customization**: Modify `docs/configuration.md` settings

Your Arduino IDE is now ready for XOOM-BA development! 🚀