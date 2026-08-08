#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>

// =====================================================
// DISPLAY
// =====================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// OLED 1 -> Wire
#define SDA1 23
#define SCL1 22

// OLED 2 -> Wire1
#define SDA2 32
#define SCL2 33

Adafruit_SSD1306 oled1(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

Adafruit_SSD1306 oled2(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire1,
  OLED_RESET
);

// =====================================================
// BME280
// =====================================================

Adafruit_BME280 bme;

// =====================================================
// BUTTONS
// =====================================================

#define BTN_LEFT   25
#define BTN_MIDDLE 26
#define BTN_RIGHT  27

// =====================================================
// BUZZER
// =====================================================

#define BUZZER 21


// =====================================================
// BEEP
// =====================================================

void beep(int duration = 60)
{
  tone(BUZZER, 2000);
  delay(duration);
  noTone(BUZZER);
}


// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("================================");
  Serial.println("      FOCUSDESK HW TEST");
  Serial.println("================================");

  // -----------------------------
  // Buttons
  // -----------------------------

  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_MIDDLE, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  // -----------------------------
  // Buzzer
  // -----------------------------

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  // -----------------------------
  // I2C BUS 1
  // OLED 1 + BME280
  // -----------------------------

  Serial.println("Starting I2C Bus 1...");

  Wire.begin(SDA1, SCL1);
  Wire.setClock(100000);

  // -----------------------------
  // I2C BUS 2
  // OLED 2
  // -----------------------------

  Serial.println("Starting I2C Bus 2...");

  Wire1.begin(SDA2, SCL2);
  Wire1.setClock(100000);

  // -----------------------------
  // OLED 1
  // -----------------------------

  Serial.println("Initializing OLED 1...");

  if (!oled1.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED 1 FAILED");

    while (1);
  }

  Serial.println("OLED 1 OK");

  // -----------------------------
  // OLED 2
  // -----------------------------

  Serial.println("Initializing OLED 2...");

  if (!oled2.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED 2 FAILED");

    while (1);
  }

  Serial.println("OLED 2 OK");

  // -----------------------------
  // BME280
  // -----------------------------

  Serial.println("Initializing BME280...");

  if (!bme.begin(0x76, &Wire))
  {
    Serial.println("BME280 FAILED");

    oled1.clearDisplay();
    oled1.setTextSize(1);
    oled1.setTextColor(SSD1306_WHITE);
    oled1.setCursor(0, 0);
    oled1.println("BME280 FAILED");
    oled1.display();

    while (1);
  }

  Serial.println("BME280 OK");

  // -----------------------------
  // Startup screen OLED 1
  // -----------------------------

  oled1.clearDisplay();

  oled1.setTextColor(SSD1306_WHITE);

  oled1.setTextSize(2);
  oled1.setCursor(5, 8);
  oled1.println("FOCUSDESK");

  oled1.setTextSize(1);
  oled1.setCursor(25, 38);
  oled1.println("Hardware Test");

  oled1.display();

  // -----------------------------
  // Startup screen OLED 2
  // -----------------------------

  oled2.clearDisplay();

  oled2.setTextColor(SSD1306_WHITE);

  oled2.setTextSize(2);
  oled2.setCursor(25, 8);
  oled2.println("READY");

  oled2.setTextSize(1);
  oled2.setCursor(15, 40);
  oled2.println("Dual I2C OK");

  oled2.display();

  // Startup beep

  beep(150);

  delay(2000);

  Serial.println("================================");
  Serial.println("ALL HARDWARE INITIALIZED");
  Serial.println("================================");
}


// =====================================================
// LOOP
// =====================================================

void loop()
{
  // ===================================================
  // READ BME280
  // ===================================================

  float temperature = bme.readTemperature();

  float humidity = bme.readHumidity();

  float pressure = bme.readPressure() / 100.0F;


  // ===================================================
  // BUTTON STATE
  // ===================================================

  bool leftPressed   = !digitalRead(BTN_LEFT);
  bool middlePressed = !digitalRead(BTN_MIDDLE);
  bool rightPressed  = !digitalRead(BTN_RIGHT);


  // ===================================================
  // OLED 1
  // SENSOR DISPLAY
  // ===================================================

  oled1.clearDisplay();

  oled1.setTextColor(SSD1306_WHITE);

  oled1.setTextSize(1);

  oled1.setCursor(0, 0);
  oled1.println("FOCUSDESK  SENSOR");

  oled1.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  oled1.setCursor(0, 16);
  oled1.print("Temp : ");
  oled1.print(temperature, 1);
  oled1.println(" C");

  oled1.setCursor(0, 28);
  oled1.print("Hum  : ");
  oled1.print(humidity, 1);
  oled1.println(" %");

  oled1.setCursor(0, 40);
  oled1.print("Press: ");
  oled1.print(pressure, 1);
  oled1.println(" hPa");

  oled1.setCursor(0, 55);
  oled1.print("I2C: BUS 1");

  oled1.display();


  // ===================================================
  // OLED 2
  // BUTTON DISPLAY
  // ===================================================

  oled2.clearDisplay();

  oled2.setTextColor(SSD1306_WHITE);

  oled2.setTextSize(1);

  oled2.setCursor(0, 0);
  oled2.println("FOCUSDESK CONTROLS");

  oled2.drawLine(0, 10, 127, 10, SSD1306_WHITE);


  oled2.setTextSize(2);

  if (leftPressed)
  {
    oled2.setCursor(20, 25);
    oled2.println("LEFT");

    Serial.println("LEFT BUTTON");

    beep();

    delay(200);
  }

  else if (middlePressed)
  {
    oled2.setCursor(15, 25);
    oled2.println("MIDDLE");

    Serial.println("MIDDLE BUTTON");

    beep();

    delay(200);
  }

  else if (rightPressed)
  {
    oled2.setCursor(15, 25);
    oled2.println("RIGHT");

    Serial.println("RIGHT BUTTON");

    beep();

    delay(200);
  }

  else
  {
    oled2.setTextSize(1);

    oled2.setCursor(20, 28);
    oled2.println("Waiting...");

    oled2.setCursor(20, 45);
    oled2.println("L    M    R");
  }

  oled2.display();


  // ===================================================
  // SERIAL MONITOR
  // ===================================================

  Serial.print("Temperature: ");
  Serial.print(temperature, 1);

  Serial.print(" C | Humidity: ");
  Serial.print(humidity, 1);

  Serial.print(" % | Pressure: ");
  Serial.print(pressure, 1);

  Serial.println(" hPa");


  delay(100);
}
