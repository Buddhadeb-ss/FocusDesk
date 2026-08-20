#ifndef POMODORO_H
#define POMODORO_H

#include <Arduino.h>

#include "display.h"
#include "buttons.h"
#include "buzzer.h"
#include "sensor.h"

class Pomodoro
{
public:

    Pomodoro(
        Display& display,
        Buttons& buttons,
        Buzzer& buzzer,
        Sensor& sensor
    );

    void begin();
    void update();

    bool wantsToExit();

private:

    enum State
    {
        MENU,
        FOCUS,
        SHORT_BREAK,
        LONG_BREAK,
        PAUSED
    };

    State _state = MENU;


    // ================================================
    // MENU
    // ================================================

    int _selection = 0;

    static const int MENU_COUNT = 3;

    const char* _menuItems[MENU_COUNT] =
    {
        "Start",
        "Settings",
        "Back"
    };


    // ================================================
    // TIMER
    // ================================================

    unsigned long _phaseStart = 0;

    unsigned long _phaseDuration = 0;

    unsigned long _pausedRemaining = 0;


    // ================================================
    // SESSIONS
    // ================================================

    int _session = 1;

    static const int TOTAL_SESSIONS = 4;


    // ================================================
    // SETTINGS
    // ================================================

    unsigned long _focusDuration =
        25UL * 60UL * 1000UL;

    unsigned long _shortBreakDuration =
        5UL * 60UL * 1000UL;

    unsigned long _longBreakDuration =
        15UL * 60UL * 1000UL;


    // ================================================
    // EXIT
    // ================================================

    bool _exitRequested = false;


    // ================================================
    // HARDWARE
    // ================================================

    Display& _display;

    Buttons& _buttons;

    Buzzer& _buzzer;

    Sensor& _sensor;


    // ================================================
    // MENU
    // ================================================

    void showMenu();

    void handleMenu();

    void moveUp();

    void moveDown();

    void selectMenu();


    // ================================================
    // TIMER
    // ================================================

    void startFocus();

    void startShortBreak();

    void startLongBreak();

    void handleTimer();

    void pauseTimer();

    void resumeTimer();

    void skipPhase();

    void phaseFinished();

    unsigned long remainingTime();


    // ================================================
    // DISPLAY
    // ================================================

    void showTimer();

    void showEnvironment();

};

#endif