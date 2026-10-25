# Wiring

## 16×2 I2C LCD Display

| LCD | ESP32-S3 |
|---|---|
| SDA | GPIO 41 |
| SCL | GPIO 40 |
| VCC | 5 V |
| GND | GND |

I2C address: `0x27`.

## OV2640 Camera

| Signal | GPIO |
|---|---|
| XCLK | 15 |
| SIOD (SDA) | 4 |
| SIOC (SCL) | 5 |
| D0 (Y2) | 11 |
| D1 (Y3) | 9 |
| D2 (Y4) | 8 |
| D3 (Y5) | 10 |
| D4 (Y6) | 12 |
| D5 (Y7) | 18 |
| D6 (Y8) | 17 |
| D7 (Y9) | 16 |
| VSYNC | 6 |
| HREF | 7 |
| PCLK | 13 |
| PWDN / RESET | Not used (`-1`) |
