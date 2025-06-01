# XOOM-BA Communication Protocols

## WiFi Access Point Handshake

```mermaid
sequenceDiagram
    participant Client as Client Device
    participant ESP32 as ESP32 Access Point
    participant DHCP as DHCP Server
    participant DNS as DNS Server
    participant Web as Web Server
    
    Note over ESP32: System Boot Sequence
    ESP32->>ESP32: Initialize WiFi Hardware
    ESP32->>ESP32: Configure AP Mode
    ESP32->>ESP32: Set SSID "DiagAP"
    ESP32->>ESP32: Start DHCP Server (192.168.4.2-100)
    ESP32->>ESP32: Start DNS Server (Port 53)
    ESP32->>ESP32: Start Web Server (Port 80)
    ESP32->>ESP32: Begin Broadcasting SSID
    
    Note over Client,ESP32: Client Connection Process
    Client->>ESP32: WiFi Scan Request
    ESP32->>Client: Beacon Frame (SSID: DiagAP)
    Client->>ESP32: Authentication Request
    ESP32->>Client: Authentication Response (Success)
    Client->>ESP32: Association Request
    ESP32->>Client: Association Response (Success)
    
    Note over Client,DHCP: DHCP IP Assignment
    Client->>DHCP: DHCP Discover Broadcast
    DHCP->>Client: DHCP Offer (IP: 192.168.4.x)
    Client->>DHCP: DHCP Request
    DHCP->>Client: DHCP ACK
    
    Note over Client,DNS: Captive Portal Detection
    Client->>DNS: DNS Query (connectivity-check.gstatic.com)
    DNS->>Client: DNS Response (192.168.4.1)
    Client->>Web: HTTP GET /generate_204
    Web->>Client: HTTP 302 Redirect to /portal
    
    Note over Client,Web: Dashboard Access
    Client->>Web: HTTP GET /portal
    Web->>Client: HTML Dashboard Page
    
    loop Every 5 seconds
        Client->>Web: AJAX GET /api/status
        Web->>Client: JSON Response (Sensors, WiFi, BT data)
    end
```

## Bluetooth Discovery Protocol

```mermaid
sequenceDiagram
    participant ESP32 as ESP32
    participant BT_Classic as Classic Bluetooth
    participant BLE_Stack as BLE Stack
    participant Target_Classic as Classic Device
    participant Target_BLE as BLE Device
    
    Note over ESP32: Bluetooth Initialization
    ESP32->>BT_Classic: Initialize Classic Bluetooth
    ESP32->>BLE_Stack: Initialize BLE Stack
    ESP32->>ESP32: Set Device Name "XOOM-BA"
    ESP32->>ESP32: Configure Scan Parameters
    
    Note over ESP32,Target_BLE: BLE Discovery Process
    ESP32->>BLE_Stack: Start BLE Scan (Active)
    BLE_Stack->>BLE_Stack: Set Scan Window: 99ms
    BLE_Stack->>BLE_Stack: Set Scan Interval: 100ms
    BLE_Stack->>BLE_Stack: Enable Active Scanning
    
    Target_BLE->>BLE_Stack: Advertisement Packet
    BLE_Stack->>ESP32: Advertisement Report
    ESP32->>ESP32: Parse Device Info (MAC, Name, RSSI)
    ESP32->>BLE_Stack: Scan Response Request (if active)
    Target_BLE->>BLE_Stack: Scan Response
    BLE_Stack->>ESP32: Scan Response Data
    ESP32->>ESP32: Store Device in List
    
    Note over ESP32,Target_Classic: Classic Bluetooth Discovery
    ESP32->>BT_Classic: Start Inquiry Scan
    BT_Classic->>BT_Classic: Set Inquiry Length: 10.24s
    BT_Classic->>BT_Classic: Set Max Responses: 20
    
    Target_Classic->>BT_Classic: Inquiry Response
    BT_Classic->>ESP32: Device Found Event
    ESP32->>ESP32: Extract Device Info
    ESP32->>BT_Classic: Remote Name Request
    Target_Classic->>BT_Classic: Remote Name Response
    BT_Classic->>ESP32: Remote Name Event
    ESP32->>ESP32: Update Device List
    
    Note over ESP32: Data Management
    ESP32->>ESP32: Remove Stale Devices (>5min)
    ESP32->>ESP32: Sort by RSSI
    ESP32->>ESP32: Update OLED Display
    ESP32->>ESP32: Update Web API Data
    
    loop Every 20 seconds
        ESP32->>BLE_Stack: Restart BLE Scan
        ESP32->>BT_Classic: Restart Classic Inquiry
    end
```

## I2C Sensor Communication

```mermaid
sequenceDiagram
    participant ESP32 as ESP32 Master
    participant BME280 as BME280 Sensor
    participant OLED as OLED Display
    
    Note over ESP32: I2C Bus Initialization
    ESP32->>ESP32: Configure GPIO21 as SDA
    ESP32->>ESP32: Configure GPIO22 as SCL
    ESP32->>ESP32: Set I2C Clock: 100kHz
    ESP32->>ESP32: Enable Internal Pull-ups
    
    Note over ESP32,BME280: BME280 Sensor Setup
    ESP32->>BME280: I2C Start + Address 0x76
    BME280->>ESP32: ACK
    ESP32->>BME280: Read Chip ID (0xD0)
    BME280->>ESP32: Chip ID (0x60)
    ESP32->>BME280: Read Calibration Data
    BME280->>ESP32: Calibration Coefficients
    ESP32->>BME280: Configure Control Registers
    BME280->>ESP32: ACK
    ESP32->>BME280: I2C Stop
    
    Note over ESP32,OLED: OLED Display Setup
    ESP32->>OLED: I2C Start + Address 0x3C
    OLED->>ESP32: ACK
    ESP32->>OLED: Command Mode (0x00)
    ESP32->>OLED: Initialization Sequence
    OLED->>ESP32: ACK
    ESP32->>OLED: Set Display ON (0xAF)
    OLED->>ESP32: ACK
    ESP32->>OLED: I2C Stop
    
    Note over ESP32,BME280: Periodic Sensor Reading
    loop Every 2 seconds
        ESP32->>BME280: I2C Start + Write Address
        BME280->>ESP32: ACK
        ESP32->>BME280: Force Measurement Command
        BME280->>ESP32: ACK
        ESP32->>BME280: I2C Stop
        
        ESP32->>ESP32: Wait 10ms (measurement time)
        
        ESP32->>BME280: I2C Start + Write Address
        BME280->>ESP32: ACK
        ESP32->>BME280: Register Address (0xF7)
        BME280->>ESP32: ACK
        ESP32->>BME280: I2C Restart + Read Address
        BME280->>ESP32: ACK
        BME280->>ESP32: Pressure Data (3 bytes)
        BME280->>ESP32: Temperature Data (3 bytes)
        BME280->>ESP32: Humidity Data (2 bytes)
        ESP32->>BME280: NACK + I2C Stop
        
        ESP32->>ESP32: Apply Calibration
        ESP32->>ESP32: Calculate Final Values
    end
    
    Note over ESP32,OLED: Display Update
    loop Every 5 seconds
        ESP32->>OLED: I2C Start + Address 0x3C
        OLED->>ESP32: ACK
        ESP32->>OLED: Data Mode (0x40)
        ESP32->>OLED: Page Buffer Data (1024 bytes)
        OLED->>ESP32: ACK (after each byte)
        ESP32->>OLED: I2C Stop
    end
```

## Web API Protocol

```mermaid
sequenceDiagram
    participant Browser as Web Browser
    participant ESP32 as ESP32 Web Server
    participant Sensors as Sensor Manager
    participant WiFi_Scanner as WiFi Scanner
    participant BT_Scanner as BT Scanner
    
    Note over Browser,ESP32: Initial Page Load
    Browser->>ESP32: HTTP GET /
    ESP32->>ESP32: Check if Captive Portal Request
    ESP32->>Browser: HTTP 200 + HTML Dashboard
    Browser->>Browser: Parse HTML + Start JavaScript
    
    Note over Browser,ESP32: Real-time Data Updates
    Browser->>ESP32: AJAX GET /api/status
    ESP32->>Sensors: Read Latest Sensor Data
    Sensors->>ESP32: Temperature, Humidity, Pressure, Light
    ESP32->>WiFi_Scanner: Get WiFi Network List
    WiFi_Scanner->>ESP32: Network Array (SSID, RSSI, Security)
    ESP32->>BT_Scanner: Get Bluetooth Device List
    BT_Scanner->>ESP32: Device Array (MAC, Name, RSSI, Type)
    ESP32->>ESP32: Build JSON Response
    ESP32->>Browser: HTTP 200 + JSON Data
    
    Browser->>Browser: Update Dashboard Elements
    Browser->>Browser: Schedule Next Request (5s timer)
    
    Note over Browser,ESP32: Individual API Endpoints
    alt Sensor Data Only
        Browser->>ESP32: GET /api/sensors
        ESP32->>Sensors: Get Current Readings
        Sensors->>ESP32: Sensor Values + Status
        ESP32->>Browser: JSON Sensor Data
    else WiFi Networks Only
        Browser->>ESP32: GET /api/wifi
        ESP32->>WiFi_Scanner: Get Network List
        WiFi_Scanner->>ESP32: WiFi Network Array
        ESP32->>Browser: JSON WiFi Data
    else Bluetooth Devices Only
        Browser->>ESP32: GET /api/bluetooth
        ESP32->>BT_Scanner: Get Device List
        BT_Scanner->>ESP32: Bluetooth Device Array
        ESP32->>Browser: JSON Bluetooth Data
    else Configuration
        Browser->>ESP32: GET /api/config
        ESP32->>ESP32: Read Current Settings
        ESP32->>Browser: JSON Configuration
    end
    
    Note over Browser,ESP32: Error Handling
    Browser->>ESP32: HTTP Request
    alt Network Error
        ESP32-->>Browser: Connection Timeout
        Browser->>Browser: Display "Connection Lost"
        Browser->>Browser: Retry in 10 seconds
    else Server Error
        ESP32->>Browser: HTTP 500 + Error Message
        Browser->>Browser: Display Error Message
    else Invalid Endpoint
        ESP32->>Browser: HTTP 404 + Not Found
        Browser->>Browser: Log Error to Console
    end
```

## System State Machine

```mermaid
stateDiagram-v2
    [*] --> Boot
    Boot --> Initializing
    Initializing --> Hardware_Check
    Hardware_Check --> Network_Setup
    Network_Setup --> Running
    
    state Hardware_Check {
        [*] --> Check_OLED
        Check_OLED --> Check_DHT22
        Check_DHT22 --> Check_BME280
        Check_BME280 --> Check_LDR
        Check_LDR --> [*]
    }
    
    state Network_Setup {
        [*] --> Start_AP
        Start_AP --> Start_DHCP
        Start_DHCP --> Start_DNS
        Start_DNS --> Start_WebServer
        Start_WebServer --> Start_Bluetooth
        Start_Bluetooth --> [*]
    }
    
    state Running {
        [*] --> Sensor_Update
        Sensor_Update --> WiFi_Scan
        WiFi_Scan --> BT_Scan
        BT_Scan --> Display_Update
        Display_Update --> Web_Handler
        Web_Handler --> Sensor_Update
        
        Sensor_Update --> Error_State : Sensor Failure
        WiFi_Scan --> Error_State : WiFi Error
        BT_Scan --> Error_State : Bluetooth Error
    }
    
    Error_State --> Recovery_Attempt
    Recovery_Attempt --> Running : Success
    Recovery_Attempt --> Restart : Failed
    Restart --> Boot
    
    Running --> Sleep_Mode : Low Power Request
    Sleep_Mode --> Running : Wake Event