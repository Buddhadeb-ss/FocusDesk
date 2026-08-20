#include "display.h"
#include "config.h"

#include <Wire.h>

static Adafruit_SSD1306 oled1(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire1,
    -1
);

static Adafruit_SSD1306 oled2(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);


bool Display::begin()
{
    Serial.println("Initializing OLED 1...");

    if (!oled1.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS))
    {
        Serial.println("OLED 1 FAILED");
        return false;
    }

    Serial.println("OLED 1 OK");


    Serial.println("Initializing OLED 2...");

    if (!oled2.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS))
    {
        Serial.println("OLED 2 FAILED");
        return false;
    }

    Serial.println("OLED 2 OK");


    oled1.clearDisplay();
    oled1.setTextColor(SSD1306_WHITE);

    oled2.clearDisplay();
    oled2.setTextColor(SSD1306_WHITE);

    oled1.display();
    oled2.display();

    return true;
}


void Display::clearLeft()
{
    oled1.clearDisplay();
}


void Display::clearRight()
{
    oled2.clearDisplay();
}


void Display::showLeft()
{
    oled1.display();
}


void Display::showRight()
{
    oled2.display();
}


Adafruit_SSD1306& Display::left()
{
    return oled1;
}


Adafruit_SSD1306& Display::right()
{
    return oled2;
}