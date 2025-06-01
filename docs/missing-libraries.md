# Missing Library Installation Guide

## ESPAsyncWebServer Installation Issue

The `ESPAsyncWebServer` library by me-no-dev is **not available** in the Arduino IDE Library Manager. This is a common issue that requires manual installation.

## Solution: Manual Installation

### Method 1: Direct ZIP Download (Recommended)

1. **Download ESPAsyncWebServer**
   - Go to: https://github.com/me-no-dev/ESPAsyncWebServer
   - Click "Code" → "Download ZIP"
   - Save as `ESPAsyncWebServer-master.zip`

2. **Download AsyncTCP (Dependency)**
   - Go to: https://github.com/me-no-dev/AsyncTCP
   - Click "Code" → "Download ZIP" 
   - Save as `AsyncTCP-master.zip`

3. **Install Libraries in Arduino IDE**
   ```
   1. Open Arduino IDE
   2. Go to Sketch → Include Library → Add .ZIP Library
   3. Select AsyncTCP-master.zip first
   4. Go to Sketch → Include Library → Add .ZIP Library again
   5. Select ESPAsyncWebServer-master.zip
   6. Restart Arduino IDE
   ```

### Method 2: Git Clone (Advanced Users)

If you have Git installed:

```bash
# Navigate to Arduino libraries folder
cd ~/Documents/Arduino/libraries/  # macOS/Linux
# or
cd %USERPROFILE%\Documents\Arduino\libraries\  # Windows

# Clone the libraries
git clone https://github.com/me-no-dev/AsyncTCP.git
git clone https://github.com/me-no-dev/ESPAsyncWebServer.git
```

### Method 3: Alternative Library (If Issues Persist)

If you continue having problems, you can use the standard ESP32 WebServer instead:

**Replace in code:**
```cpp
// Instead of:
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

// Use:
#include <WebServer.h>
WebServer server(80);
```

## Arduino Libraries Folder Location

### Find Your Libraries Folder:
- **Windows**: `C:\Users\[username]\Documents\Arduino\libraries\`
- **macOS**: `/Users/[username]/Documents/Arduino/libraries/`
- **Linux**: `/home/[username]/Arduino/libraries/`

### Or check in Arduino IDE:
1. File → Preferences
2. Look at "Sketchbook location"
3. Libraries folder is inside that directory

## Verification Steps

1. **Check Installation**
   - Restart Arduino IDE
   - Go to Sketch → Include Library
   - You should see "ESPAsyncWebServer" and "AsyncTCP" in the list

2. **Test Compilation**
   ```cpp
   #include <ESPAsyncWebServer.h>
   #include <AsyncTCP.h>
   
   AsyncWebServer server(80);
   
   void setup() {
     // Test compilation
   }
   void loop() {}
   ```

3. **Compile Test**
   - Create new sketch with above code
   - Select ESP32 board
   - Click Verify (✓)
   - Should compile without errors

## Common Issues & Solutions

### Issue 1: "Library not found"
**Solution:**
- Ensure both AsyncTCP and ESPAsyncWebServer are installed
- Restart Arduino IDE
- Check libraries folder permissions

### Issue 2: Compilation errors
**Solution:**
- Install AsyncTCP **before** ESPAsyncWebServer
- Use latest ESP32 core (2.0.0+)
- Clear Arduino cache and restart

### Issue 3: Version conflicts
**Solution:**
- Remove old versions from libraries folder
- Download fresh copies from GitHub
- Ensure no duplicate library names

## Alternative: Use Standard WebServer

If async libraries cause persistent issues, the XOOM-BA code can be modified to use the standard ESP32 WebServer:

### Code Changes Required:
```cpp
// Replace async includes
#include <WebServer.h>
#include <DNSServer.h>

// Replace async objects  
WebServer server(80);
DNSServer dnsServer;

// Replace async handlers
server.on("/", HTTP_GET, handleRoot);
server.on("/api/status", HTTP_GET, handleAPIStatus);
// etc.
```

This approach sacrifices some performance but ensures compatibility.

## Support Resources

- **ESPAsyncWebServer GitHub**: https://github.com/me-no-dev/ESPAsyncWebServer
- **AsyncTCP GitHub**: https://github.com/me-no-dev/AsyncTCP
- **ESP32 Arduino Core**: https://github.com/espressif/arduino-esp32
- **Arduino Forum**: https://forum.arduino.cc/