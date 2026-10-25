# Embedded Facial Recognition and Object Detection (ESP32)

An embedded AI (TinyML) system running on the **ESP32-S3**. A camera captures images, an **Edge Impulse** model runs directly on the microcontroller, and the results (class label + confidence score) are displayed on a **16×2 I2C LCD**.

## Features

- Real-time image capture (OV2640 camera, QVGA 320×240)
- On-device inference using Edge Impulse (no cloud, no Wi-Fi required)
- Display of the predicted label and confidence score on a 16×2 LCD
- Output of inference results via the serial port (115200 baud)
- Support for two modes: image classification and object detection

## Hardware

| Component | Details |
|---|---|
| Board | ESP32-S3 with OPI PSRAM (ESP32-S3-EYE-style pinout) |
| Camera | OV2640 |
| Display | 16×2 I2C LCD (address `0x27`) |

See [`hardware/bill_of_materials.md`](hardware/bill_of_materials.md) and [`docs/wiring.md`](docs/wiring.md) for detailed information.

## Repository Structure

```text
firmware/esp32_edge_ai_detection/   Arduino code and board configuration
model/                              Edge Impulse model information
docs/                               Wiring diagrams and images
hardware/                           Bill of materials
```

## Installation

1. Install **Arduino IDE** and the **esp32 by Espressif** board package.
2. Install the **LiquidCrystal I2C** library through the Library Manager.
3. Import the exported Edge Impulse library:

   `Sketch > Include Library > Add .ZIP Library`

   See [`model/README.md`](model/README.md) for details.

4. Select the **ESP32S3 Dev Module** board and configure **PSRAM: OPI PSRAM**. The complete configuration is available in `sketch.json`.
5. Open `firmware/esp32_edge_ai_detection/`, compile the code, and upload it to the board.
6. Open the Serial Monitor at **115200 baud**.

## How It Works

1. Initialize the LCD and camera.
2. In the main loop, capture a JPEG image, convert it to RGB888, and resize it to match the model's input dimensions.
3. Run `run_classifier()` to perform inference using Edge Impulse.
4. Display the predicted label and confidence score (`%`) on the LCD and output the results through the serial port.

## Author

**Lalili Mohamed Ramzi**

- LinkedIn: https://www.linkedin.com/in/ramzi-lalili-2948a8310/
- Email: ramzilalili@gmail.com

## License

MIT License. See [`LICENSE`](LICENSE) for details.
