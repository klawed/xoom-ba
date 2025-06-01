# Configuration Options

## Network Configuration

```cpp
// WiFi Access Point Settings
#define AP_SSID "DiagAP"              // Access Point name
#define AP_PASSWORD ""                // Leave empty for open network
#define AP_CHANNEL 1                  // WiFi channel (1-13)
#define AP_HIDDEN false               // Hide SSID broadcast
#define MAX_CONNECTIONS 4             // Maximum connected clients

// IP Configuration
IPAddress local_ip(192, 168, 4, 1);   // Access Point IP
IPAddress gateway(192, 168, 4, 1);    // Gateway IP
IPAddress subnet(255, 255, 255, 0);   // Subnet mask
```

## Hardware Configuration

```cpp
// Display Settings
#define SCREEN_WIDTH 128              // OLED width in pixels
#define SCREEN_HEIGHT 64              // OLED height in pixels
#define OLED_RESET -1                 // Reset pin (-1 if sharing reset)
#define SCREEN_ADDRESS 0x3C           // I2C address (0x3C or 0x3D)

// Pin Assignments
#define DHT_PIN 4                     // DHT22 data pin
#define DHT_TYPE DHT22                // DHT sensor type
#define LDR_PIN 34                    // Light sensor analog pin
#define BUTTON_PIN 0                  // Push button pin
#define LED_PIN 2                     // Status LED pin

// I2C Configuration
#define I2C_SDA 21                    // I2C data pin
#define I2C_SCL 22                    // I2C clock pin
#define I2C_FREQUENCY 100000          // I2C clock frequency (Hz)
```

## Timing Configuration

```cpp
// Update Intervals (milliseconds)
#define DISPLAY_UPDATE_INTERVAL 5000  // Time between display pages
#define SENSOR_UPDATE_INTERVAL 2000   // Sensor reading frequency
#define WIFI_SCAN_INTERVAL 30000      // WiFi network scan frequency
#define BLE_SCAN_INTERVAL 20000       // Bluetooth scan frequency
#define WEB_UPDATE_INTERVAL 1000      // Web interface refresh rate

// Timeout Settings
#define SENSOR_TIMEOUT 5000           // Sensor read timeout
#define NETWORK_TIMEOUT 10000         // Network operation timeout
#define WEB_REQUEST_TIMEOUT 30000     // Web request timeout
```

## Sensor Configuration

```cpp
// DHT22 Settings
#define DHT_RETRIES 3                 // Read retry attempts
#define DHT_MIN_INTERVAL 2000         // Minimum read interval

// BME280 Settings
#define BME280_ADDRESS_PRIMARY 0x76   // Primary I2C address
#define BME280_ADDRESS_SECONDARY 0x77 // Alternative I2C address

// Light Sensor Calibration
#define LDR_MIN_VALUE 0               // Minimum ADC reading
#define LDR_MAX_VALUE 4095            // Maximum ADC reading
#define LUX_MIN_VALUE 0               // Corresponding minimum lux
#define LUX_MAX_VALUE 1000            // Corresponding maximum lux
```

## Display Configuration

```cpp
// Page Display Settings
enum DisplayPage {
  PAGE_SENSORS,                       // Sensor readings page
  PAGE_WIFI,                          // WiFi networks page
  PAGE_BLUETOOTH,                     // Bluetooth devices page
  PAGE_AP_INFO,                       // Access point info page
  PAGE_SYSTEM,                        // System information page
  PAGE_COUNT                          // Total page count
};

// Display Limits
#define MAX_WIFI_DISPLAY 4            // Max WiFi networks on screen
#define MAX_BT_DISPLAY 3              // Max Bluetooth devices on screen
#define MAX_WIFI_STORED 20            // Max WiFi networks in memory
#define MAX_BT_STORED 20              // Max Bluetooth devices in memory
```

## Bluetooth Configuration

```cpp
// BLE Scan Settings
#define BLE_SCAN_TIME 5               // Scan duration (seconds)
#define BLE_SCAN_INTERVAL 100         // Scan interval (0.625ms units)
#define BLE_SCAN_WINDOW 99            // Scan window (0.625ms units)
#define BLE_ACTIVE_SCAN true          // Active vs passive scanning

// Classic Bluetooth Settings
#define BT_INQUIRY_LENGTH 10240       // Inquiry length (1.28s units)
#define BT_MAX_RESPONSES 20           // Maximum inquiry responses

// Device Aging
#define DEVICE_MAX_AGE 300000         // Remove devices after 5 minutes
```

## Web Server Configuration

```cpp
// HTTP Server Settings
#define WEB_SERVER_PORT 80            // Web server port
#define DNS_SERVER_PORT 53            // DNS server port for captive portal
#define MAX_CLIENTS 4                 // Maximum concurrent web clients

// JSON Response Limits
#define JSON_BUFFER_SIZE 4096         // JSON document buffer size
#define API_RESPONSE_LIMIT 2048       // Maximum API response size
```

## Power Management

```cpp
// CPU Settings
#define CPU_FREQUENCY 240             // CPU frequency (MHz)
#define WIFI_POWER_SAVE false         // WiFi power save mode
#define BT_POWER_SAVE false           // Bluetooth power save mode

// Sleep Settings (for battery operation)
#define LIGHT_SLEEP_ENABLE false      // Enable light sleep
#define DEEP_SLEEP_ENABLE false       // Enable deep sleep
#define SLEEP_DURATION 60000000       // Sleep duration (microseconds)
```

## Security Configuration

```cpp
// Access Control
#define ENABLE_WEB_AUTH false         // Enable web authentication
#define WEB_USERNAME "admin"          // Web interface username
#define WEB_PASSWORD "password"       // Web interface password

// API Security
#define ENABLE_API_KEY false          // Require API key
#define API_KEY "your-api-key-here"   // API access key
#define RATE_LIMITING false           // Enable request rate limiting
#define MAX_REQUESTS_PER_MINUTE 60    // Rate limit threshold
```

## Debug Configuration

```cpp
// Serial Debug
#define DEBUG_SERIAL true             // Enable serial debug output
#define SERIAL_BAUD 115200            // Serial baud rate
#define DEBUG_LEVEL 1                 // Debug verbosity (0-3)

// LED Debug Indicators
#define DEBUG_LED true                // Enable LED status indicators
#define LED_BOOT_TIME 2000            // LED on time during boot
#define LED_ERROR_PATTERN true        // Blink pattern for errors
```

## Customization Examples

### Change Access Point Name and Password
```cpp
#define AP_SSID "MyCustomAP"
#define AP_PASSWORD "MySecurePassword123"
```

### Adjust Sensor Update Frequency
```cpp
#define SENSOR_UPDATE_INTERVAL 1000   // Update every 1 second
#define DISPLAY_UPDATE_INTERVAL 3000  // Change pages every 3 seconds
```

### Modify Display Content
```cpp
// Add custom pages to DisplayPage enum
enum DisplayPage {
  PAGE_SENSORS,
  PAGE_WIFI,
  PAGE_BLUETOOTH,
  PAGE_AP_INFO,
  PAGE_SYSTEM,
  PAGE_CUSTOM1,    // Add custom page
  PAGE_COUNT
};
```

### Change Pin Assignments
```cpp
#define DHT_PIN 5                     // Move DHT22 to GPIO5
#define LDR_PIN 35                    // Move LDR to GPIO35
#define BUTTON_PIN 12                 // Move button to GPIO12
```

### Battery Optimization
```cpp
#define CPU_FREQUENCY 80              // Reduce CPU frequency
#define WIFI_POWER_SAVE true          // Enable WiFi power save
#define SENSOR_UPDATE_INTERVAL 10000  // Update sensors less frequently
#define WIFI_SCAN_INTERVAL 120000     // Scan WiFi less frequently
```

## Configuration Validation

The system performs basic validation of configuration values:

- **Pin conflicts**: Ensures no GPIO pins are assigned to multiple functions
- **Memory limits**: Validates buffer sizes don't exceed available RAM
- **Timing constraints**: Ensures update intervals are reasonable
- **I2C addresses**: Checks for address conflicts between devices
- **Power limits**: Warns if configuration may exceed power budget

## Runtime Configuration

Some settings can be modified at runtime through the web interface:

- **Display update interval**
- **Sensor polling frequency**
- **Network scan intervals**
- **Debug output level**
- **Power save modes**

These changes are stored in ESP32 preferences and persist across reboots.