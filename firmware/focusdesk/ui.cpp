#include "ui.h"


UI::UI(
    Display& display,
    Buttons& buttons,
    Sensor& sensor,
    Buzzer& buzzer,
    EnvironmentApp& environmentApp,
    App* apps[],
    int appCount
)
    : _display(display),
      _buttons(buttons),
      _sensor(sensor),
      _buzzer(buzzer),
      _environmentApp(environmentApp),
      _apps(apps),
      _appCount(appCount)
{
}


// ====================================================
// BEGIN
// ====================================================

void UI::begin()
{
    _screen = HOME;

    _menuSelection = 0;

    _menuTop = 0;

    showHome();
}


// ====================================================
// UPDATE
// ====================================================

void UI::update()
{
    // =================================================
    // UNIVERSAL LONG HOLD → HOME
    // =================================================

    if (_buttons.middleLongHeld())
    {
        _screen = HOME;

        _buzzer.beep(2500, 200);

        showHome();

        return;
    }


    // =================================================
    // HOME
    // =================================================

    if (_screen == HOME)
    {
        if (_buttons.middleHeld())
        {
            _screen = MENU;

            _menuSelection = 0;
            _menuTop = 0;

            _buzzer.beep(2500, 200);

            showMenu();

            return;
        }

        static unsigned long lastHomeUpdate = 0;

        if (millis() - lastHomeUpdate >= 1000)
        {
            lastHomeUpdate = millis();

            showHome();
        }

        return;
    }


    // =================================================
    // MENU
    // =================================================

    if (_screen == MENU)
    {
        if (_buttons.leftPressed())
        {
            moveMenuUp();
            _buzzer.beep();
            showMenu();
            return;
        }

        if (_buttons.rightPressed())
        {
            moveMenuDown();
            _buzzer.beep();
            showMenu();
            return;
        }

        if (_buttons.middlePressed())
        {
            selectMenuItem();
            return;
        }

        return;
    }


    // =================================================
    // APPLICATION
    // =================================================

    if (_screen == APP)
    {
        updateApp();
        return;
    }
}


// ====================================================
// MENU UP
// ====================================================

void UI::moveMenuUp()
{
    _menuSelection--;

    if (_menuSelection < 0)
    {
        _menuSelection = _appCount - 1;
    }

    if (_menuSelection < _menuTop)
    {
        _menuTop = _menuSelection;
    }
}


// ====================================================
// MENU DOWN
// ====================================================

void UI::moveMenuDown()
{
    _menuSelection++;

    if (_menuSelection >= _appCount)
    {
        _menuSelection = 0;
        _menuTop = 0;
        return;
    }

    if (_menuSelection >= _menuTop + MENU_VISIBLE)
    {
        _menuTop = _menuSelection - MENU_VISIBLE + 1;
    }
}


// ====================================================
// SELECT MENU ITEM
// ====================================================

void UI::selectMenuItem()
{
    _currentAppIndex = _menuSelection;

    App* app = _apps[_currentAppIndex];

    Serial.print("Opening: ");
    Serial.println(app->getName());

    _buzzer.beep();

    _screen = APP;

    app->begin();
}


// ====================================================
// APPLICATION UPDATE
// ====================================================

void UI::updateApp()
{
    App* app = _apps[_currentAppIndex];

    app->update();

    if (app->wantsToExit())
    {
        _screen = HOME;

        // Reset menu cursor too, so the next time MENU is
        // opened it doesn't silently resume wherever it was
        // left — this is the "clean reset" part of the fix.

        _menuSelection = 0;
        _menuTop = 0;

        showHome();
    }
}


// ====================================================
// HOME
// ====================================================

void UI::showHome()
{
    // Left OLED

    _display.left().clearDisplay();
    _display.left().setTextColor(SSD1306_WHITE);

    _display.left().setTextSize(2);
    _display.left().setCursor(5, 8);
    _display.left().println("FOCUSDESK");

    _display.left().setTextSize(2);
    _display.left().setCursor(25, 38);
    _display.left().println("WELCOME");

    _display.left().display();

    // Right OLED — reuse EnvironmentApp's shared readout
    // instead of duplicating the drawing code here.

    _environmentApp.showReadout();
}


// ====================================================
// MENU
// ====================================================

void UI::showMenu()
{
    // Left OLED

    _display.left().clearDisplay();
    _display.left().setTextColor(SSD1306_WHITE);

    _display.left().setTextSize(2);
    _display.left().setCursor(0, 0);
    _display.left().println("MENU");

    _display.left().setTextSize(1);

    for (int i = 0; i < MENU_VISIBLE; i++)
    {
        int itemIndex = _menuTop + i;

        if (itemIndex >= _appCount)
        {
            break;
        }

        int y = 25 + (i * 13);

        _display.left().setCursor(0, y);

        if (itemIndex == _menuSelection)
        {
            _display.left().print("> ");
        }
        else
        {
            _display.left().print("  ");
        }

        _display.left().println(_apps[itemIndex]->getName());
    }

    _display.left().display();

    // Right OLED

    _display.right().clearDisplay();
    _display.right().setTextColor(SSD1306_WHITE);
    _display.right().setTextSize(1);

    _display.right().setCursor(0, 3);
    _display.right().println("MENU NAVIGATION");

    _display.right().drawLine(0, 13, 127, 13, SSD1306_WHITE);

    _display.right().setCursor(0, 20);
    _display.right().println("LEFT  - UP");

    _display.right().setCursor(0, 32);
    _display.right().println("RIGHT - DOWN");

    _display.right().setCursor(0, 44);
    _display.right().println("MIDDLE - SELECT");

    _display.right().setCursor(0, 56);
    _display.right().println("HOLD 2s - HOME");

    _display.right().display();
}
