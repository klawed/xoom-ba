# XOOM-BA Hardware Configuration

## Pin Assignments

| Component | ESP32 Pin | Function | Notes |
|-----------|-----------|----------|-------|
| OLED VCC | 3.3V | Power | |
| OLED GND | GND | Ground | |
| OLED SDA | GPIO21 | I2C Data | Shared with BME280 |
| OLED SCL | GPIO22 | I2C Clock | Shared with BME280 |
| DHT22 VCC | 3.3V | Power | |
| DHT22 GND | GND | Ground | |
| DHT22 Data | GPIO4 | Digital Input | |
| BME280 VCC | 3.3V | Power | |
| BME280 GND | GND | Ground | |
| BME280 SDA | GPIO21 | I2C Data | Shared with OLED |
| BME280 SCL | GPIO22 | I2C Clock | Shared with OLED |
| LDR (High) | 3.3V | Power | Through voltage divider |
| LDR (Low) | GPIO34 | Analog Input | ADC1_CH6 |
| LDR Resistor | GND | Ground | 10kΩ pull-down |
| Button | GPIO0 | Digital Input | Boot button, pull-up enabled |
| Button GND | GND | Ground | |
| Status LED | GPIO2 | Digital Output | Built-in LED |
| LED Resistor | GND | Ground | 220Ω current limiting |

## I2C Device Addresses

| Device | Default Address | Alternative Address |
|--------|----------------|-------------------|
| SSD1306 OLED | 0x3C | 0x3D |
| BME280 | 0x76 | 0x77 |

## Power Requirements

- **ESP32**: 3.3V, ~240mA (peak with WiFi/BT)
- **OLED Display**: 3.3V, ~20mA
- **DHT22**: 3.3V, ~2.5mA
- **BME280**: 3.3V, ~3.6µA (sleep), ~714µA (active)
- **Total**: ~300mA (peak operation)

## Recommended Power Supply

- **USB**: 5V/1A USB adapter or power bank
- **Battery**: 3.7V LiPo with voltage regulator
- **External**: 5-12V DC with ESP32 onboard regulator