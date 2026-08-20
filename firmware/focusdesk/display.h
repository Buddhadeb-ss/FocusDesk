#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

class Display
{
public:
    bool begin();

    void clearLeft();
    void clearRight();

    void showLeft();
    void showRight();

    Adafruit_SSD1306& left();
    Adafruit_SSD1306& right();
};

#endif