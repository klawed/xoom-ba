# XOOM-BA Circuit Schematics

## Complete System Schematic

```mermaid
graph TB
    subgraph "ESP32 DevKit v1"
        ESP32[ESP32 Microcontroller]
        USB[USB Connector]
        REG[3.3V Regulator]
        RST[Reset Button]
        BOOT[Boot Button/GPIO0]
        LED[Built-in LED/GPIO2]
    end
    
    subgraph "Display Module"
        OLED[128x64 OLED SSD1306]
        OLED_VCC[VCC]
        OLED_GND[GND] 
        OLED_SDA[SDA]
        OLED_SCL[SCL]
    end
    
    subgraph "Environmental Sensors"
        DHT22[DHT22 Temp/Humidity]
        DHT_VCC[VCC]
        DHT_GND[GND]
        DHT_DATA[DATA]
        
        BME280[BME280 Pressure/Temp/Humidity]
        BME_VCC[VCC]
        BME_GND[GND]
        BME_SDA[SDA]
        BME_SCL[SCL]
    end
    
    subgraph "Light Sensor Circuit"
        LDR[Light Dependent Resistor]
        R1[10kΩ Resistor]
    end
    
    subgraph "User Interface"
        BTN[Push Button]
        BTN_GND[Button GND]
        EXT_LED[External LED]
        R2[220Ω Resistor]
    end
    
    subgraph "Power & I2C Bus"
        VCC_BUS[3.3V Bus]
        GND_BUS[Ground Bus]
        SDA_BUS[I2C SDA Bus]
        SCL_BUS[I2C SCL Bus]
    end
    
    %% Power connections
    ESP32 ---|3.3V| VCC_BUS
    ESP32 ---|GND| GND_BUS
    ESP32 ---|GPIO21| SDA_BUS
    ESP32 ---|GPIO22| SCL_BUS
    ESP32 ---|GPIO4| DHT_DATA
    ESP32 ---|GPIO34| LDR
    ESP32 ---|GPIO0| BTN
    ESP32 ---|GPIO2| EXT_LED
    
    %% OLED connections
    VCC_BUS --- OLED_VCC
    GND_BUS --- OLED_GND
    SDA_BUS --- OLED_SDA
    SCL_BUS --- OLED_SCL
    
    %% DHT22 connections
    VCC_BUS --- DHT_VCC
    GND_BUS --- DHT_GND
    
    %% BME280 connections  
    VCC_BUS --- BME_VCC
    GND_BUS --- BME_GND
    SDA_BUS --- BME_SDA
    SCL_BUS --- BME_SCL
    
    %% Light sensor voltage divider
    VCC_BUS --- LDR
    LDR --- R1
    R1 --- GND_BUS
    
    %% Button and LED
    BTN --- BTN_GND
    BTN_GND --- GND_BUS
    EXT_LED --- R2
    R2 --- GND_BUS
```

## Power Distribution Schematic

```mermaid
graph LR
    subgraph "Power Input"
        USB_5V[USB 5V]
        VIN[VIN Pin]
        BATT[Battery Input]
    end
    
    subgraph "ESP32 Board"
        AMS1117[AMS1117 3.3V LDO]
        ESP32_CHIP[ESP32 Chip]
        BYPASS[100µF + 10µF Capacitors]
    end
    
    subgraph "Load Distribution"
        OLED_LOAD[OLED: 20mA]
        DHT_LOAD[DHT22: 2.5mA]
        BME_LOAD[BME280: 3.6µA-714µA]
        ESP_LOAD[ESP32: 80-240mA]
    end
    
    USB_5V --> AMS1117
    VIN --> AMS1117
    BATT --> AMS1117
    
    AMS1117 --> ESP32_CHIP
    AMS1117 --> BYPASS
    AMS1117 --> OLED_LOAD
    AMS1117 --> DHT_LOAD
    AMS1117 --> BME_LOAD
    
    ESP32_CHIP --> ESP_LOAD
```

## I2C Bus Topology

```mermaid
graph TD
    ESP32[ESP32<br/>GPIO21: SDA<br/>GPIO22: SCL]
    
    subgraph "I2C Bus"
        SDA[SDA Line<br/>4.7kΩ Pull-up]
        SCL[SCL Line<br/>4.7kΩ Pull-up]
    end
    
    subgraph "I2C Devices"
        OLED[OLED Display<br/>Address: 0x3C]
        BME[BME280 Sensor<br/>Address: 0x76]
    end
    
    ESP32 ---|GPIO21| SDA
    ESP32 ---|GPIO22| SCL
    ESP32 ---|3.3V| SDA
    ESP32 ---|3.3V| SCL
    
    SDA --- OLED
    SCL --- OLED
    SDA --- BME
    SCL --- BME
```

## Analog Input Circuit

```mermaid
graph TD
    VCC[3.3V]
    LDR[Light Dependent Resistor<br/>1kΩ - 1MΩ]
    R1[Fixed Resistor<br/>10kΩ]
    ADC[ESP32 GPIO34<br/>ADC1_CH6]
    GND[Ground]
    
    VCC --- LDR
    LDR --- ADC
    LDR --- R1
    R1 --- GND
    
    style LDR fill:#ffeb3b
    style ADC fill:#4caf50
```

## PCB Layout Recommendations

```mermaid
graph TB
    subgraph "Layer 1 - Component Placement"
        ESP32_PCB[ESP32 Module<br/>Center-Left]
        OLED_PCB[OLED Display<br/>Top-Center]
        DHT_PCB[DHT22<br/>Top-Right]
        BME_PCB[BME280<br/>Bottom-Right]
        LDR_PCB[LDR Circuit<br/>Bottom-Left]
        CONN_PCB[Connectors<br/>Edges]
    end
    
    subgraph "Layer 2 - Ground Plane"
        GND_PLANE[Solid Ground Plane<br/>Maximum Coverage]
    end
    
    subgraph "Layer 3 - Power & I2C"
        VCC_TRACE[3.3V Power Traces<br/>Thick, Star Pattern]
        I2C_TRACE[I2C Bus Traces<br/>Short, Equal Length]
    end
    
    subgraph "Layer 4 - Signals"
        GPIO_TRACE[GPIO Signal Traces<br/>Digital & Analog]
    end
```

## Component Values and Specifications

| Component | Value/Part Number | Tolerance | Package | Notes |
|-----------|------------------|-----------|---------|-------|
| R1 (LDR Pulldown) | 10kΩ | ±5% | 0805 | Metal film preferred |
| R2 (LED Current) | 220Ω | ±5% | 0805 | For 3.3V operation |
| C1 (Power Bypass) | 100µF | ±20% | Electrolytic | Tantalum alternative |
| C2 (High Freq Bypass) | 10µF | ±10% | Ceramic | X7R dielectric |
| C3 (I2C Decoupling) | 100nF | ±10% | Ceramic | Near each I2C device |
| LDR | GL5528 | - | Through-hole | 5-10kΩ @ 10 lux |
| Button | Tactile Switch | - | 6x6mm | 50mA, 12V rated |

## Assembly Notes

1. **Soldering Order**: Start with smallest components first
2. **I2C Bus**: Keep traces short and equal length
3. **Power Decoupling**: Place bypass capacitors close to power pins
4. **Ground Plane**: Ensure solid ground connections
5. **Testing Points**: Add test points for key signals
6. **Mechanical**: Consider mounting holes and enclosure compatibility