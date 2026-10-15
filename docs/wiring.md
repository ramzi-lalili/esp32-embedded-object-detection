# Câblage

## Écran LCD I2C 16x2

| LCD | ESP32-S3 |
|---|---|
| SDA | GPIO 41 |
| SCL | GPIO 40 |
| VCC | 5 V |
| GND | GND |

Adresse I2C : `0x27`.

## Caméra OV2640

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
| PWDN / RESET | non utilisés (-1) |

Ajouter ici un schéma ou une photo du montage dans `docs/images/`.
