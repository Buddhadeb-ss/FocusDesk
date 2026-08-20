# Hardware Connection

Wiring reference for the FocusDesk ESP32 build. Pin numbers below are pulled directly from `firmware/config.h` and cross-checked against how each peripheral is actually initialized in `display.cpp` / `sensor.cpp` / `buttons.cpp` / `buzzer.cpp` — not just the config comments (see note below on why that distinction matters here).

---

## ⚠️ Note on I2C bus assignment

`config.h`'s comments label the buses like this:

```
I2C Bus 1 (Wire)  → OLED 1 + BME280
I2C Bus 2 (Wire1) → OLED 2
```

But the actual driver code wires it the other way around for the OLEDs:

- **`Display::left()`** (the object called `oled1` in code) is initialized on **`Wire1`** — I2C Bus 2
- **`Display::right()`** (`oled2` in code) is initialized on **`Wire`** — I2C Bus 1, shared with the BME280

The table below documents the **real wiring** (what the code does), not the comment. If you're wiring this from scratch, follow the table, not the comment in `config.h` — worth fixing that comment eventually so it doesn't mislead the next person (including future you).

---

## I2C Bus 1 — `Wire` (pins 23 / 22)

Shared bus. Both devices sit at different I2C addresses on the same two wires.

| Signal | ESP32 Pin | Connects to |
|--------|-----------|-------------|
| SDA    | GPIO 23   | Right OLED (`Display::right()`) SDA + BME280 SDA |
| SCL    | GPIO 22   | Right OLED (`Display::right()`) SCL + BME280 SCL |

**Devices on this bus:**
- SSD1306 OLED (right display) — I2C address `0x3C`
- BME280 environment sensor — I2C address `0x76`

---

## I2C Bus 2 — `Wire1` (pins 32 / 33)

Dedicated bus for the second display only.

| Signal | ESP32 Pin | Connects to |
|--------|-----------|-------------|
| SDA    | GPIO 32   | Left OLED (`Display::left()`) SDA |
| SCL    | GPIO 33   | Left OLED (`Display::left()`) SCL |

**Devices on this bus:**
- SSD1306 OLED (left display) — I2C address `0x3C`

> Both OLEDs share the same I2C address (`0x3C`). That's exactly why they're split across two separate I2C buses instead of one shared bus — otherwise they'd collide.

---

## Buttons

All three buttons use `INPUT_PULLUP`, meaning each pin is pulled HIGH internally and reads LOW when pressed. Wire each button between its GPIO pin and GND — no external resistor needed.

| Button  | ESP32 Pin | Wiring |
|---------|-----------|--------|
| Left    | GPIO 25   | Pin ↔ button ↔ GND |
| Middle  | GPIO 26   | Pin ↔ button ↔ GND |
| Right   | GPIO 27   | Pin ↔ button ↔ GND |

Middle button also supports hold (750 ms) and long-hold (2000 ms) detection in firmware — no extra wiring required, that's handled in `buttons.cpp` by timing how long the pin stays LOW.

---

## Buzzer

| Signal | ESP32 Pin | Notes |
|--------|-----------|-------|
| Signal | GPIO 21   | Passive piezo buzzer, driven with `tone()` |

Driven directly as a digital output — connect buzzer's positive lead to GPIO 21, negative to GND. If using a piezo that draws more current than a GPIO pin can safely source, drive it through a transistor instead of wiring it directly.

---

## Power

Not covered by `config.h` — depends on your specific board/regulator setup. General notes:
- ESP32 dev boards typically run I2C peripherals (OLEDs, BME280) off 3.3V
- Confirm your specific SSD1306 and BME280 breakout boards are 3.3V-tolerant before connecting to the ESP32's 3.3V rail (most common breakout modules are, but check the specific one you have)
- Buttons and buzzer are simple GPIO — no separate power rail needed beyond the ESP32 itself

---

## I2C Address Reference

| Device | Address |
|--------|---------|
| Both SSD1306 OLEDs | `0x3C` |
| BME280 | `0x76` |

If your BME280 breakout uses `0x77` instead (some do, depending on the SDO pin state), update `BME280_ADDRESS` in `config.h` accordingly.

---

## WiFi credentials — security note

`config.h` currently stores `WIFI_SSID` and `WIFI_PASSWORD` as plaintext `#define`s directly in the source file. That file is tracked in this repo, which means committing it as-is pushes real WiFi credentials into git history — including past commits, even if removed later.

If continuing this project: move those two lines into a separate file (e.g. `secrets.h`), add that file to `.gitignore`, and commit a `secrets.h.example` with placeholder values instead. This doc intentionally does not reproduce the actual credential values from `config.h`.
