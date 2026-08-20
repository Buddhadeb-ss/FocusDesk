#include "PlaceholderApp.h"


PlaceholderApp::PlaceholderApp(
    Display& display,
    Buttons& buttons,
    Buzzer& buzzer,
    const char* name
)
    : _display(display),
      _buttons(buttons),
      _buzzer(buzzer),
      _name(name)
{
}


// ====================================================
// BEGIN
// ====================================================

void PlaceholderApp::begin()
{
    show();
}


// ====================================================
// UPDATE
// ====================================================

void PlaceholderApp::update()
{
    if (_buttons.leftPressed())
    {
        _buzzer.beep();
        return;
    }

    if (_buttons.rightPressed())
    {
        _buzzer.beep();
        return;
    }

    if (_buttons.middlePressed())
    {
        _buzzer.beep();
        return;
    }
}


// ====================================================
// NAME
// ====================================================

const char* PlaceholderApp::getName()
{
    return _name;
}


// ====================================================
// SHOW
// ====================================================

void PlaceholderApp::show()
{
    // Left OLED

    _display.left().clearDisplay();
    _display.left().setTextColor(SSD1306_WHITE);

    _display.left().setTextSize(2);
    _display.left().setCursor(0, 5);
    _display.left().println(_name);

    _display.left().setTextSize(1);
    _display.left().setCursor(0, 32);
    _display.left().println("APPLICATION");

    _display.left().setCursor(0, 47);
    _display.left().println("Coming soon...");

    _display.left().display();

    // Right OLED

    _display.right().clearDisplay();
    _display.right().setTextColor(SSD1306_WHITE);
    _display.right().setTextSize(1);

    _display.right().setCursor(0, 3);
    _display.right().println("APP NAVIGATION");

    _display.right().drawLine(0, 13, 127, 13, SSD1306_WHITE);

    _display.right().setCursor(0, 21);
    _display.right().println("LEFT  - Previous");

    _display.right().setCursor(0, 33);
    _display.right().println("RIGHT - Next");

    _display.right().setCursor(0, 45);
    _display.right().println("MIDDLE- Select");

    _display.right().setCursor(0, 57);
    _display.right().println("HOLD M- Home");

    _display.right().display();
}
