#include "IPWebApp.h"


IPWebApp::IPWebApp(
    Display& display,
    WiFiService& wifi
)
    : _display(display),
      _wifi(wifi)
{
}


// ====================================================
// BEGIN
// ====================================================

void IPWebApp::begin()
{
    _lastUpdate = 0;

    show();
}


// ====================================================
// UPDATE
// ====================================================

void IPWebApp::update()
{
    if (millis() - _lastUpdate >= UPDATE_INTERVAL)
    {
        _lastUpdate = millis();

        show();
    }
}


// ====================================================
// NAME
// ====================================================

const char* IPWebApp::getName()
{
    return "IP / Web";
}


// ====================================================
// SHOW
// ====================================================

void IPWebApp::show()
{
    // Left OLED

    _display.left().clearDisplay();
    _display.left().setTextColor(SSD1306_WHITE);

    _display.left().setTextSize(2);
    _display.left().setCursor(0, 0);
    _display.left().println("IP / WEB");

    _display.left().drawLine(0, 18, 127, 18, SSD1306_WHITE);

    _display.left().setTextSize(1);
    _display.left().setCursor(0, 28);
    _display.left().println("IP ADDRESS:");

    _display.left().setCursor(0, 42);
    _display.left().println(_wifi.ipAddress());

    _display.left().setCursor(0, 56);
    _display.left().print("Mode: ");
    _display.left().println(_wifi.mode());

    _display.left().display();

    // Right OLED

    _display.right().clearDisplay();
    _display.right().setTextColor(SSD1306_WHITE);
    _display.right().setTextSize(1);

    _display.right().setCursor(0, 0);
    _display.right().println("WEB SERVER");

    _display.right().drawLine(0, 10, 127, 10, SSD1306_WHITE);

    _display.right().setCursor(0, 20);

    if (_wifi.connected())
    {
        _display.right().println("STATUS: ONLINE");
    }
    else
    {
        _display.right().println("STATUS: OFFLINE");
    }

    _display.right().setCursor(0, 36);
    _display.right().println("Open browser:");

    _display.right().setCursor(0, 50);
    _display.right().println(_wifi.ipAddress());

    _display.right().display();
}
