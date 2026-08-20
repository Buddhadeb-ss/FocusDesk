#ifndef CONFIG_H
#define CONFIG_H

// =========================
// I2C BUS 1
// OLED 1 + BME280
// =========================

#define I2C1_SDA 23
#define I2C1_SCL 22

// =========================
// I2C BUS 2
// OLED 2
// =========================

#define I2C2_SDA 32
#define I2C2_SCL 33

// =========================
// Buttons
// =========================

#define BTN_LEFT   25
#define BTN_MIDDLE 26
#define BTN_RIGHT  27

// =========================
// Buzzer
// =========================

#define BUZZER_PIN 21

// =========================
// I2C Addresses
// =========================

#define OLED_ADDRESS 0x3C
#define BME280_ADDRESS 0x76

#endif

// ====================================================
// WiFi
// ====================================================

#define WIFI_SSID "WIFI_ID"
#define WIFI_PASSWORD "WIFI_PASS"
