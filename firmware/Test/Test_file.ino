#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define SDA_PIN 23
#define SCL_PIN 22

#define BTN_LEFT   25
#define BTN_MID    26
#define BTN_RIGHT  27

#define BUZZER     21

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BME280 bme;

void beep(int freq = 2000, int duration = 60)
{
  tone(BUZZER, freq);
  delay(duration);
  noTone(BUZZER);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_MID, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED FAILED");
    while (1);
  }

  if (!bme.begin(0x76))
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("BME280 FAILED");
    display.display();

    Serial.println("BME280 FAILED");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(8, 10);
  display.println("FocusDesk");
  display.setTextSize(1);
  display.setCursor(25, 40);
  display.println("Hardware Test");
  display.display();

  beep(2500, 150);

  delay(2000);
}

void loop()
{
  float temp = bme.readTemperature();
  float hum = bme.readHumidity();
  float pres = bme.readPressure() / 100.0;

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("=== FocusDesk ===");

  display.print("Temp : ");
  display.print(temp, 1);
  display.println(" C");

  display.print("Hum  : ");
  display.print(hum, 1);
  display.println(" %");

  display.print("Pres : ");
  display.print(pres, 1);
  display.println(" hPa");

  display.print("Btn  : ");

  if (!digitalRead(BTN_LEFT))
  {
    display.println("LEFT");
    Serial.println("LEFT");
    beep();
    delay(200);
  }
  else if (!digitalRead(BTN_MID))
  {
    display.println("MIDDLE");
    Serial.println("MIDDLE");
    beep();
    delay(200);
  }
  else if (!digitalRead(BTN_RIGHT))
  {
    display.println("RIGHT");
    Serial.println("RIGHT");
    beep();
    delay(200);
  }
  else
  {
    display.println("NONE");
  }

  display.display();

  Serial.print("T: ");
  Serial.print(temp);
  Serial.print(" C   H: ");
  Serial.print(hum);
  Serial.print(" %   P: ");
  Serial.print(pres);
  Serial.println(" hPa");

  delay(100);
}