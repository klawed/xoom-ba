/*
 * XOOM-BA: ESP32 Network & Sensor Diagnostics Access Point
 * 
 * Features:
 * - WiFi Access Point with captive portal
 * - Continuous WiFi and Bluetooth scanning
 * - Multiple sensor monitoring (DHT22, BME280, LDR)
 * - OLED display with rotating pages
 * - RESTful API for remote monitoring
 * - Real-time web dashboard
 */

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_BME280.h>
#include <BluetoothSerial.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>

// Hardware Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define DHT_PIN 4
#define DHT_TYPE DHT22
#define LDR_PIN 34
#define BUTTON_PIN 0
#define LED_PIN 2

// Network Configuration
#define AP_SSID "DiagAP"
#define AP_PASSWORD ""  // Open network
#define AP_CHANNEL 1
#define AP_HIDDEN false
#define MAX_CONNECTIONS 4

// Timing Constants
#define DISPLAY_UPDATE_INTERVAL 5000  // 5 seconds between pages
#define SENSOR_UPDATE_INTERVAL 2000   // 2 seconds for sensors
#define WIFI_SCAN_INTERVAL 30000      // 30 seconds for WiFi scan
#define BLE_SCAN_INTERVAL 20000       // 20 seconds for BLE scan
#define WEB_UPDATE_INTERVAL 1000      // 1 second for web updates

// Display Pages
enum DisplayPage {
  PAGE_SENSORS,
  PAGE_WIFI,
  PAGE_BLUETOOTH,
  PAGE_AP_INFO,
  PAGE_SYSTEM,
  PAGE_COUNT
};

// Sensor Data Structure
struct SensorData {
  float temperature = 0.0;
  float humidity = 0.0;
  float pressure = 0.0;
  float light = 0.0;
  bool dht_ok = false;
  bool bme_ok = false;
  unsigned long last_update = 0;
};

// WiFi Network Structure
struct WiFiNetwork {
  String ssid;
  int32_t rssi;
  String security;
  int32_t channel;
  String mac;
};

// Bluetooth Device Structure
struct BluetoothDevice {
  String mac;
  String name;
  int rssi;
  String type;
  unsigned long last_seen;
};

// Global Objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BME280 bme;
WebServer server(80);
DNSServer dnsServer;
BLEScan* pBLEScan;

// Global Variables
SensorData sensors;
std::vector<WiFiNetwork> wifi_networks;
std::vector<BluetoothDevice> bluetooth_devices;
DisplayPage current_page = PAGE_SENSORS;
unsigned long last_display_update = 0;
unsigned long last_sensor_update = 0;
unsigned long last_wifi_scan = 0;
unsigned long last_ble_scan = 0;
unsigned long last_button_press = 0;
unsigned long system_start_time = 0;
bool button_pressed = false;
int connected_clients = 0;

// BLE Scan Callback Class
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    BluetoothDevice device;
    device.mac = advertisedDevice.getAddress().toString().c_str();
    device.name = advertisedDevice.getName().c_str();
    device.rssi = advertisedDevice.getRSSI();
    device.type = "BLE";
    device.last_seen = millis();
    
    // Update or add device to list
    bool found = false;
    for (auto& existing : bluetooth_devices) {
      if (existing.mac == device.mac) {
        existing = device;
        found = true;
        break;
      }
    }
    
    if (!found && bluetooth_devices.size() < 20) {
      bluetooth_devices.push_back(device);
    }
  }
};

void setup() {
  Serial.begin(115200);
  system_start_time = millis();
  
  // Initialize Hardware
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, HIGH);  // Turn on status LED
  
  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("XOOM-BA Starting..."));
  display.display();
  
  // Initialize Sensors
  dht.begin();
  sensors.bme_ok = bme.begin(0x76) || bme.begin(0x77);  // Try both addresses
  
  if (sensors.bme_ok) {
    Serial.println("BME280 sensor found");
  } else {
    Serial.println("Could not find BME280 sensor");
  }
  
  // Initialize WiFi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD, AP_CHANNEL, AP_HIDDEN, MAX_CONNECTIONS);
  
  IPAddress local_ip(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  
  Serial.println("Access Point Started");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  
  // Initialize DNS Server for Captive Portal
  dnsServer.start(53, "*", WiFi.softAPIP());
  
  // Initialize Bluetooth
  BLEDevice::init("XOOM-BA");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  
  // Initialize Web Server
  setupWebServer();
  server.begin();
  
  Serial.println("System initialization complete");
  digitalWrite(LED_PIN, LOW);  // Turn off status LED
}

void loop() {
  unsigned long current_time = millis();
  
  // Handle DNS requests for captive portal
  dnsServer.processNextRequest();
  
  // Handle web server requests
  server.handleClient();
  
  // Check button press
  checkButton();
  
  // Update sensors
  if (current_time - last_sensor_update >= SENSOR_UPDATE_INTERVAL) {
    updateSensors();
    last_sensor_update = current_time;
  }
  
  // Scan WiFi networks
  if (current_time - last_wifi_scan >= WIFI_SCAN_INTERVAL) {
    scanWiFiNetworks();
    last_wifi_scan = current_time;
  }
  
  // Scan Bluetooth devices
  if (current_time - last_ble_scan >= BLE_SCAN_INTERVAL) {
    scanBluetoothDevices();
    last_ble_scan = current_time;
  }
  
  // Update display
  if (current_time - last_display_update >= DISPLAY_UPDATE_INTERVAL || button_pressed) {
    updateDisplay();
    last_display_update = current_time;
    button_pressed = false;
  }
  
  // Update connected clients count
  connected_clients = WiFi.softAPgetStationNum();
  
  delay(100);  // Small delay to prevent watchdog reset
}

void checkButton() {
  if (digitalRead(BUTTON_PIN) == LOW && millis() - last_button_press > 500) {
    current_page = (DisplayPage)((current_page + 1) % PAGE_COUNT);
    button_pressed = true;
    last_button_press = millis();
    Serial.println("Button pressed, page changed");
  }
}

void updateSensors() {
  // Read DHT22
  sensors.temperature = dht.readTemperature();
  sensors.humidity = dht.readHumidity();
  sensors.dht_ok = !isnan(sensors.temperature) && !isnan(sensors.humidity);
  
  // Read BME280
  if (sensors.bme_ok) {
    sensors.pressure = bme.readPressure() / 100.0F;  // Convert to hPa
    if (!sensors.dht_ok) {  // Use BME280 temp/humidity if DHT failed
      sensors.temperature = bme.readTemperature();
      sensors.humidity = bme.readHumidity();
    }
  }
  
  // Read LDR
  int ldr_reading = analogRead(LDR_PIN);
  sensors.light = map(ldr_reading, 0, 4095, 0, 1000);  // Convert to lux approximation
  
  sensors.last_update = millis();
}

void scanWiFiNetworks() {
  int n = WiFi.scanNetworks();
  wifi_networks.clear();
  
  for (int i = 0; i < n && i < 10; ++i) {  // Limit to 10 networks
    WiFiNetwork network;
    network.ssid = WiFi.SSID(i);
    network.rssi = WiFi.RSSI(i);
    network.channel = WiFi.channel(i);
    network.mac = WiFi.BSSIDstr(i);
    
    wifi_encryption_type_t encryption = WiFi.encryptionType(i);
    switch (encryption) {
      case WIFI_AUTH_OPEN: network.security = "Open"; break;
      case WIFI_AUTH_WEP: network.security = "WEP"; break;
      case WIFI_AUTH_WPA_PSK: network.security = "WPA"; break;
      case WIFI_AUTH_WPA2_PSK: network.security = "WPA2"; break;
      case WIFI_AUTH_WPA_WPA2_PSK: network.security = "WPA/WPA2"; break;
      case WIFI_AUTH_WPA2_ENTERPRISE: network.security = "WPA2-ENT"; break;
      default: network.security = "Unknown"; break;
    }
    
    wifi_networks.push_back(network);
  }
  
  Serial.printf("Found %d WiFi networks\n", wifi_networks.size());
}

void scanBluetoothDevices() {
  // Remove old devices (older than 5 minutes)
  unsigned long current_time = millis();
  bluetooth_devices.erase(
    std::remove_if(bluetooth_devices.begin(), bluetooth_devices.end(),
      [current_time](const BluetoothDevice& device) {
        return (current_time - device.last_seen) > 300000;  // 5 minutes
      }),
    bluetooth_devices.end()
  );
  
  // Start BLE scan
  BLEScanResults foundDevices = pBLEScan->start(5, false);  // Scan for 5 seconds
  pBLEScan->clearResults();
  
  Serial.printf("Found %d Bluetooth devices\n", bluetooth_devices.size());
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  switch (current_page) {
    case PAGE_SENSORS:
      displaySensors();
      break;
    case PAGE_WIFI:
      displayWiFiNetworks();
      break;
    case PAGE_BLUETOOTH:
      displayBluetoothDevices();
      break;
    case PAGE_AP_INFO:
      displayAPInfo();
      break;
    case PAGE_SYSTEM:
      displaySystemInfo();
      break;
  }
  
  display.display();
}

void displaySensors() {
  display.println("[ SENSORS ]");
  display.println();
  
  if (sensors.dht_ok || sensors.bme_ok) {
    display.printf("Temp: %.1f°C\n", sensors.temperature);
    display.printf("Humidity: %.1f%%\n", sensors.humidity);
  } else {
    display.println("Temp: ERROR");
    display.println("Humidity: ERROR");
  }
  
  if (sensors.bme_ok) {
    display.printf("Pressure: %.1fhPa\n", sensors.pressure);
  } else {
    display.println("Pressure: N/A");
  }
  
  display.printf("Light: %.0f lux\n", sensors.light);
  
  display.println();
  display.printf("Updated: %lus ago\n", (millis() - sensors.last_update) / 1000);
}

void displayWiFiNetworks() {
  display.println("[ WIFI NETWORKS ]");
  display.println();
  
  if (wifi_networks.empty()) {
    display.println("Scanning...");
    return;
  }
  
  int max_display = min(4, (int)wifi_networks.size());
  for (int i = 0; i < max_display; i++) {
    display.printf("%d.%s\n", i+1, wifi_networks[i].ssid.c_str());
    display.printf("  %ddBm %s Ch%d\n", 
                   wifi_networks[i].rssi, 
                   wifi_networks[i].security.c_str(),
                   wifi_networks[i].channel);
  }
  
  if (wifi_networks.size() > 4) {
    display.printf("...and %d more\n", wifi_networks.size() - 4);
  }
}

void displayBluetoothDevices() {
  display.println("[ BLUETOOTH ]");
  display.println();
  
  if (bluetooth_devices.empty()) {
    display.println("Scanning...");
    return;
  }
  
  int max_display = min(3, (int)bluetooth_devices.size());
  for (int i = 0; i < max_display; i++) {
    String name = bluetooth_devices[i].name;
    if (name.length() == 0) {
      name = "Unknown";
    }
    if (name.length() > 12) {
      name = name.substring(0, 12) + "...";
    }
    
    display.printf("%d.%s\n", i+1, name.c_str());
    display.printf("  %s %ddBm\n", 
                   bluetooth_devices[i].type.c_str(),
                   bluetooth_devices[i].rssi);
  }
  
  if (bluetooth_devices.size() > 3) {
    display.printf("...and %d more\n", bluetooth_devices.size() - 3);
  }
}

void displayAPInfo() {
  display.println("[ ACCESS POINT ]");
  display.println();
  
  display.printf("SSID: %s\n", AP_SSID);
  display.printf("IP: %s\n", WiFi.softAPIP().toString().c_str());
  display.printf("Clients: %d/%d\n", connected_clients, MAX_CONNECTIONS);
  display.printf("Channel: %d\n", AP_CHANNEL);
  display.println();
  display.println("Connect & browse to");
  display.println("access web interface");
}

void displaySystemInfo() {
  display.println("[ SYSTEM INFO ]");
  display.println();
  
  unsigned long uptime = (millis() - system_start_time) / 1000;
  display.printf("Uptime: %lus\n", uptime);
  display.printf("Free RAM: %d bytes\n", ESP.getFreeHeap());
  display.printf("CPU Freq: %dMHz\n", ESP.getCpuFreqMHz());
  display.printf("Flash: %dMB\n", ESP.getFlashChipSize() / (1024 * 1024));
  display.println();
  display.printf("WiFi: %d networks\n", wifi_networks.size());
  display.printf("BT: %d devices\n", bluetooth_devices.size());
}

void setupWebServer() {
  // Captive Portal - redirect all requests to main page
  server.onNotFound([]() {
    if (server.hostHeader() != WiFi.softAPIP().toString()) {
      server.sendHeader("Location", "http://" + WiFi.softAPIP().toString(), true);
      server.send(302, "text/plain", "");
    } else {
      handleRoot();
    }
  });
  
  // Main dashboard
  server.on("/", handleRoot);
  server.on("/portal", handleRoot);
  
  // API endpoints
  server.on("/api/status", handleAPIStatus);
  server.on("/api/sensors", handleAPISensors);
  server.on("/api/wifi", handleAPIWiFi);
  server.on("/api/bluetooth", handleAPIBluetooth);
  server.on("/api/config", HTTP_GET, handleAPIConfig);
  
  Serial.println("Web server configured");
}

void handleRoot() {
  String html = generateHTML();
  server.send(200, "text/html", html);
}

void handleAPIStatus() {
  DynamicJsonDocument doc(4096);
  
  doc["timestamp"] = millis();
  doc["uptime"] = (millis() - system_start_time) / 1000;
  
  // Sensor data
  JsonObject sensor_obj = doc.createNestedObject("sensors");
  sensor_obj["temperature"] = sensors.temperature;
  sensor_obj["humidity"] = sensors.humidity;
  sensor_obj["pressure"] = sensors.pressure;
  sensor_obj["light"] = sensors.light;
  sensor_obj["dht_ok"] = sensors.dht_ok;
  sensor_obj["bme_ok"] = sensors.bme_ok;
  
  // WiFi networks
  JsonArray wifi_array = doc.createNestedArray("wifi");
  for (const auto& network : wifi_networks) {
    JsonObject wifi_obj = wifi_array.createNestedObject();
    wifi_obj["ssid"] = network.ssid;
    wifi_obj["rssi"] = network.rssi;
    wifi_obj["security"] = network.security;
    wifi_obj["channel"] = network.channel;
    wifi_obj["mac"] = network.mac;
  }
  
  // Bluetooth devices
  JsonArray bt_array = doc.createNestedArray("bluetooth");
  for (const auto& device : bluetooth_devices) {
    JsonObject bt_obj = bt_array.createNestedObject();
    bt_obj["mac"] = device.mac;
    bt_obj["name"] = device.name;
    bt_obj["rssi"] = device.rssi;
    bt_obj["type"] = device.type;
    bt_obj["last_seen"] = device.last_seen;
  }
  
  // System info
  JsonObject system_obj = doc.createNestedObject("system");
  system_obj["free_heap"] = ESP.getFreeHeap();
  system_obj["wifi_clients"] = connected_clients;
  system_obj["ap_ip"] = WiFi.softAPIP().toString();
  system_obj["cpu_freq"] = ESP.getCpuFreqMHz();
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleAPISensors() {
  DynamicJsonDocument doc(1024);
  
  doc["temperature"] = sensors.temperature;
  doc["humidity"] = sensors.humidity;
  doc["pressure"] = sensors.pressure;
  doc["light"] = sensors.light;
  doc["dht_ok"] = sensors.dht_ok;
  doc["bme_ok"] = sensors.bme_ok;
  doc["last_update"] = sensors.last_update;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleAPIWiFi() {
  DynamicJsonDocument doc(2048);
  JsonArray array = doc.to<JsonArray>();
  
  for (const auto& network : wifi_networks) {
    JsonObject obj = array.createNestedObject();
    obj["ssid"] = network.ssid;
    obj["rssi"] = network.rssi;
    obj["security"] = network.security;
    obj["channel"] = network.channel;
    obj["mac"] = network.mac;
  }
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleAPIBluetooth() {
  DynamicJsonDocument doc(2048);
  JsonArray array = doc.to<JsonArray>();
  
  for (const auto& device : bluetooth_devices) {
    JsonObject obj = array.createNestedObject();
    obj["mac"] = device.mac;
    obj["name"] = device.name;
    obj["rssi"] = device.rssi;
    obj["type"] = device.type;
    obj["last_seen"] = device.last_seen;
  }
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleAPIConfig() {
  DynamicJsonDocument doc(1024);
  
  doc["ap_ssid"] = AP_SSID;
  doc["ap_channel"] = AP_CHANNEL;
  doc["max_connections"] = MAX_CONNECTIONS;
  doc["display_interval"] = DISPLAY_UPDATE_INTERVAL;
  doc["sensor_interval"] = SENSOR_UPDATE_INTERVAL;
  doc["wifi_scan_interval"] = WIFI_SCAN_INTERVAL;
  doc["ble_scan_interval"] = BLE_SCAN_INTERVAL;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

String generateHTML() {
  return R"HTML(
<!DOCTYPE html>
<html>
<head>
    <title>XOOM-BA Dashboard</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; background: #f0f0f0; }
        .container { max-width: 800px; margin: 0 auto; }
        .card { background: white; padding: 20px; margin: 10px 0; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }
        .header { text-align: center; color: #333; }
        .grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); gap: 20px; }
        .metric { display: flex; justify-content: space-between; margin: 10px 0; }
        .value { font-weight: bold; color: #007acc; }
        .list-item { padding: 8px; border-bottom: 1px solid #eee; }
        .rssi { font-size: 0.9em; color: #666; }
        .refresh-btn { background: #007acc; color: white; border: none; padding: 10px 20px; border-radius: 4px; cursor: pointer; }
        .status-online { color: #28a745; }
        .status-error { color: #dc3545; }
    </style>
</head>
<body>
    <div class="container">
        <div class="card">
            <h1 class="header">🔍 XOOM-BA Network Diagnostics</h1>
            <p style="text-align: center;">ESP32 Access Point • Live Monitoring Dashboard</p>
        </div>
        
        <div class="grid">
            <div class="card">
                <h3>📊 Sensors</h3>
                <div id="sensors">Loading...</div>
            </div>
            
            <div class="card">
                <h3>📡 WiFi Networks</h3>
                <div id="wifi">Scanning...</div>
            </div>
            
            <div class="card">
                <h3>🔵 Bluetooth Devices</h3>
                <div id="bluetooth">Scanning...</div>
            </div>
            
            <div class="card">
                <h3>⚙️ System Info</h3>
                <div id="system">Loading...</div>
            </div>
        </div>
        
        <div class="card">
            <button class="refresh-btn" onclick="loadData()">🔄 Refresh Data</button>
            <p><small>Auto-refresh every 5 seconds • <span id="timestamp"></span></small></p>
        </div>
    </div>

    <script>
        function loadData() {
            fetch('/api/status')
                .then(response => response.json())
                .then(data => {
                    updateSensors(data.sensors);
                    updateWiFi(data.wifi);
                    updateBluetooth(data.bluetooth);
                    updateSystem(data.system, data.uptime);
                    document.getElementById('timestamp').textContent = new Date().toLocaleTimeString();
                })
                .catch(error => console.error('Error:', error));
        }
        
        function updateSensors(sensors) {
            const html = `
                <div class="metric">
                    <span>Temperature:</span>
                    <span class="value">${sensors.temperature.toFixed(1)}°C</span>
                </div>
                <div class="metric">
                    <span>Humidity:</span>
                    <span class="value">${sensors.humidity.toFixed(1)}%</span>
                </div>
                <div class="metric">
                    <span>Pressure:</span>
                    <span class="value">${sensors.pressure.toFixed(1)} hPa</span>
                </div>
                <div class="metric">
                    <span>Light:</span>
                    <span class="value">${sensors.light.toFixed(0)} lux</span>
                </div>
                <div class="metric">
                    <span>DHT22:</span>
                    <span class="${sensors.dht_ok ? 'status-online' : 'status-error'}">${sensors.dht_ok ? 'OK' : 'ERROR'}</span>
                </div>
                <div class="metric">
                    <span>BME280:</span>
                    <span class="${sensors.bme_ok ? 'status-online' : 'status-error'}">${sensors.bme_ok ? 'OK' : 'N/A'}</span>
                </div>
            `;
            document.getElementById('sensors').innerHTML = html;
        }
        
        function updateWiFi(networks) {
            if (networks.length === 0) {
                document.getElementById('wifi').innerHTML = '<p>No networks found</p>';
                return;
            }
            
            let html = '';
            networks.forEach((network, index) => {
                html += `
                    <div class="list-item">
                        <strong>${network.ssid}</strong><br>
                        <span class="rssi">${network.rssi} dBm • ${network.security} • Ch${network.channel}</span>
                    </div>
                `;
            });
            document.getElementById('wifi').innerHTML = html;
        }
        
        function updateBluetooth(devices) {
            if (devices.length === 0) {
                document.getElementById('bluetooth').innerHTML = '<p>No devices found</p>';
                return;
            }
            
            let html = '';
            devices.forEach((device, index) => {
                const name = device.name || 'Unknown Device';
                html += `
                    <div class="list-item">
                        <strong>${name}</strong><br>
                        <span class="rssi">${device.type} • ${device.rssi} dBm</span>
                    </div>
                `;
            });
            document.getElementById('bluetooth').innerHTML = html;
        }
        
        function updateSystem(system, uptime) {
            const hours = Math.floor(uptime / 3600);
            const minutes = Math.floor((uptime % 3600) / 60);
            const seconds = uptime % 60;
            
            const html = `
                <div class="metric">
                    <span>Uptime:</span>
                    <span class="value">${hours}h ${minutes}m ${seconds}s</span>
                </div>
                <div class="metric">
                    <span>Free RAM:</span>
                    <span class="value">${(system.free_heap / 1024).toFixed(1)} KB</span>
                </div>
                <div class="metric">
                    <span>Connected Clients:</span>
                    <span class="value">${system.wifi_clients}</span>
                </div>
                <div class="metric">
                    <span>AP IP Address:</span>
                    <span class="value">${system.ap_ip}</span>
                </div>
                <div class="metric">
                    <span>CPU Frequency:</span>
                    <span class="value">${system.cpu_freq} MHz</span>
                </div>
            `;
            document.getElementById('system').innerHTML = html;
        }
        
        // Load data on page load and set up auto-refresh
        loadData();
        setInterval(loadData, 5000);  // Refresh every 5 seconds
    </script>
</body>
</html>
)HTML";
}
