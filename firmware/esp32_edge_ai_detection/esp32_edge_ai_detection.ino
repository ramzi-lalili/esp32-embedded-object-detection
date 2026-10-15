/*
 * Reconnaissance faciale et détection d'objets embarquées (ESP32-S3)
 * -------------------------------------------------------------------
 * Capture caméra (OV2640, QVGA) -> inférence Edge Impulse -> affichage LCD I2C 16x2.
 *
 * Carte    : ESP32S3 Dev Module (PSRAM OPI)
 * Librairie: <SECURTI_inferencing.h> (export Edge Impulse, voir model/README.md)
 */
#include <SECURTI_inferencing.h>
#include "edge-impulse-sdk/dsp/image/image.hpp"
#include "esp_camera.h"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/************ LCD CONFIG ************/
#define LCD_SDA 41
#define LCD_SCL 40
LiquidCrystal_I2C lcd(0x27, 16, 2);

/************ CAMERA CONFIG (ESP-EYE) ************/
#define CAMERA_MODEL_ESP_EYE

#if defined(CAMERA_MODEL_ESP_EYE)
#define PWDN_GPIO_NUM  -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM  15
#define SIOD_GPIO_NUM  4
#define SIOC_GPIO_NUM  5

#define Y2_GPIO_NUM 11
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16

#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM  7
#define PCLK_GPIO_NUM  13
#endif

/************ CONSTANTS ************/
#define EI_CAMERA_RAW_FRAME_BUFFER_COLS 320
#define EI_CAMERA_RAW_FRAME_BUFFER_ROWS 240
#define EI_CAMERA_FRAME_BYTE_SIZE 3

/************ GLOBALS ************/
uint8_t *snapshot_buf;
static bool is_initialised = false;

/************ CAMERA STRUCT ************/
camera_config_t camera_config = {
    .pin_pwdn       = PWDN_GPIO_NUM,
    .pin_reset      = RESET_GPIO_NUM,
    .pin_xclk       = XCLK_GPIO_NUM,
    .pin_sscb_sda   = SIOD_GPIO_NUM,
    .pin_sscb_scl   = SIOC_GPIO_NUM,

    .pin_d7         = Y9_GPIO_NUM,
    .pin_d6         = Y8_GPIO_NUM,
    .pin_d5         = Y7_GPIO_NUM,
    .pin_d4         = Y6_GPIO_NUM,
    .pin_d3         = Y5_GPIO_NUM,
    .pin_d2         = Y4_GPIO_NUM,
    .pin_d1         = Y3_GPIO_NUM,
    .pin_d0         = Y2_GPIO_NUM,
    .pin_vsync      = VSYNC_GPIO_NUM,
    .pin_href       = HREF_GPIO_NUM,
    .pin_pclk       = PCLK_GPIO_NUM,

    .xclk_freq_hz   = 20000000,
    .ledc_timer     = LEDC_TIMER_0,
    .ledc_channel   = LEDC_CHANNEL_0,

    .pixel_format   = PIXFORMAT_JPEG,
    .frame_size     = FRAMESIZE_QVGA,
    .jpeg_quality   = 12,
    .fb_count       = 1,
    .fb_location    = CAMERA_FB_IN_PSRAM,
    .grab_mode      = CAMERA_GRAB_WHEN_EMPTY
};

/************ PROTOTYPES ************/
bool ei_camera_init(void);
bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf);
static int ei_camera_get_data(size_t offset, size_t length, float *out_ptr);


/******************************************************
 *  SETUP
 ******************************************************/
void setup() {
    Serial.begin(115200);
    while (!Serial);

    /************ INIT LCD ************/
    Wire.begin(LCD_SDA, LCD_SCL);
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Edge Impulse");
    lcd.setCursor(0,1);
    lcd.print("Starting...");

    /************ INIT CAMERA ************/
    if (!ei_camera_init()) {
        Serial.println("Camera init failed!");
        lcd.clear();
        lcd.print("Camera Error");
        while (1);
    }

    lcd.clear();
    lcd.print("Cam Ready");
    delay(1000);
    lcd.clear();
}

/******************************************************
 *  MAIN LOOP
 ******************************************************/
void loop() {

    if (ei_sleep(5) != EI_IMPULSE_OK) return;

    snapshot_buf = (uint8_t*)malloc(
        EI_CAMERA_RAW_FRAME_BUFFER_COLS * 
        EI_CAMERA_RAW_FRAME_BUFFER_ROWS * 
        EI_CAMERA_FRAME_BYTE_SIZE
    );

    if (!snapshot_buf) {
        Serial.println("Memory Error!");
        return;
    }

    if (!ei_camera_capture(EI_CLASSIFIER_INPUT_WIDTH,
                           EI_CLASSIFIER_INPUT_HEIGHT,
                           snapshot_buf)) {
        Serial.println("Capture failed!");
        free(snapshot_buf);
        return;
    }

    /************ RUN INFERENCE ************/
    ei_impulse_result_t result = { 0 };
    ei::signal_t signal;
    signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
    signal.get_data = &ei_camera_get_data;

    EI_IMPULSE_ERROR err = run_classifier(&signal, &result, false);
    if (err != EI_IMPULSE_OK) {
        Serial.println("Classifier failed!");
        free(snapshot_buf);
        return;
    }

#if EI_CLASSIFIER_OBJECT_DETECTION == 1

    bool detected = false;

    for (int i = 0; i < result.bounding_boxes_count; i++) {

        auto bb = result.bounding_boxes[i];
        if (bb.value == 0) continue;

        detected = true;

        Serial.printf("Detected: %s (%.2f)\n", bb.label, bb.value);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(bb.label);

        lcd.setCursor(0,1);
        lcd.print("Conf:");
        lcd.print((int)(bb.value * 100));
        lcd.print("%");
    }

    if (!detected) {
        lcd.clear();
        lcd.print("No Object");
    }

#else

    int best_i = 0;
    float best_v = 0;

    for (int i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        if (result.classification[i].value > best_v) {
            best_v = result.classification[i].value;
            best_i = i;
        }
    }

    Serial.printf("Detected: %s (%.2f)\n",
        ei_classifier_inferencing_categories[best_i], best_v);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(ei_classifier_inferencing_categories[best_i]);

    lcd.setCursor(0,1);
    lcd.print("Conf:");
    lcd.print((int)(best_v * 100));
    lcd.print("%");

#endif

    free(snapshot_buf);
  delay(1000);

}

/******************************************************
 *  CAMERA INIT
 ******************************************************/
bool ei_camera_init(void) {

    if (is_initialised) return true;

    if (esp_camera_init(&camera_config) != ESP_OK) return false;

    sensor_t *s = esp_camera_sensor_get();
    s->set_vflip(s, 1);
    s->set_hmirror(s, 1);

    is_initialised = true;
    return true;
}


/******************************************************
 *  CAMERA CAPTURE
 ******************************************************/
bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf) {

    if (!is_initialised) return false;

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) return false;

    bool ok = fmt2rgb888(fb->buf, fb->len, PIXFORMAT_JPEG, snapshot_buf);
    esp_camera_fb_return(fb);

    if (!ok) return false;

    if (img_width != EI_CAMERA_RAW_FRAME_BUFFER_COLS ||
        img_height != EI_CAMERA_RAW_FRAME_BUFFER_ROWS) {

        ei::image::processing::crop_and_interpolate_rgb888(
            out_buf,
            EI_CAMERA_RAW_FRAME_BUFFER_COLS,
            EI_CAMERA_RAW_FRAME_BUFFER_ROWS,
            out_buf,
            img_width,
            img_height
        );
    }

    return true;
}


/******************************************************
 *  EI GET DATA
 ******************************************************/
static int ei_camera_get_data(size_t offset, size_t length, float *out_ptr) {

    size_t pixel = offset * 3;

    for (size_t i = 0; i < length; i++) {
        out_ptr[i] = (snapshot_buf[pixel + 2] << 16) |
                     (snapshot_buf[pixel + 1] << 8) |
                     (snapshot_buf[pixel + 0]);
        pixel += 3;
    }

    return 0;
}

