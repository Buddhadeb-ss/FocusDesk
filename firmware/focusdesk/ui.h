#ifndef UI_H
#define UI_H

#include "display.h"
#include "buttons.h"
#include "sensor.h"
#include "buzzer.h"
#include "app_interface.h"
#include "EnvironmentApp.h"


class UI
{
public:

    UI(
        Display& display,
        Buttons& buttons,
        Sensor& sensor,
        Buzzer& buzzer,
        EnvironmentApp& environmentApp,
        App* apps[],
        int appCount
    );


    void begin();

    void update();


private:

    // ================================================
    // SCREENS
    // ================================================

    enum Screen
    {
        HOME,
        MENU,
        APP
    };


    Screen _screen = HOME;


    // ================================================
    // APPLICATIONS
    // ================================================
    // UI no longer knows what any individual app does —
    // it just holds pointers to them and calls the
    // App interface (begin/update/wantsToExit/getName).
    // To add a new app, add it to the array passed in
    // from FocusDesk.ino. Nothing in this file changes.
    // ================================================

    App** _apps;

    int _appCount;

    int _currentAppIndex = 0;


    // ================================================
    // MENU
    // ================================================

    static const int MENU_VISIBLE = 3;


    int _menuSelection = 0;

    int _menuTop = 0;


    // ================================================
    // HARDWARE
    // ================================================

    Display& _display;

    Buttons& _buttons;

    Sensor& _sensor;

    Buzzer& _buzzer;


    // ================================================
    // ENVIRONMENT (shared readout used on HOME screen)
    // ================================================

    EnvironmentApp& _environmentApp;


    // ================================================
    // HOME
    // ================================================

    void showHome();


    // ================================================
    // MENU
    // ================================================

    void showMenu();

    void moveMenuUp();

    void moveMenuDown();

    void selectMenuItem();


    // ================================================
    // APPLICATION
    // ================================================

    void updateApp();
};

#endif
