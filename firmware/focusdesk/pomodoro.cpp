#include "pomodoro.h"


Pomodoro::Pomodoro(
    Display& display,
    Buttons& buttons,
    Buzzer& buzzer,
    Sensor& sensor
)
    : _display(display),
      _buttons(buttons),
      _buzzer(buzzer),
      _sensor(sensor)
{
}


// ====================================================
// BEGIN
// ====================================================

void Pomodoro::begin()
{
    _state = MENU;

    _selection = 0;

    _session = 1;

    _phaseStart = 0;

    _phaseDuration = 0;

    _pausedRemaining = 0;

    _exitRequested = false;

    showMenu();
}


// ====================================================
// UPDATE
// ====================================================

void Pomodoro::update()
{
    // NOTE: _exitRequested is intentionally NOT reset here.
    // It used to be cleared at the top of every update() call,
    // which meant it was wiped out on the very same frame it
    // could have been set — UI::updateApp() would never see it
    // turn true, so "Back" could never actually exit the app.
    // It's now only cleared in begin(), when the app is opened
    // fresh, and set true below when the user picks "Back".

    // Menu

    if (_state == MENU)
    {
        handleMenu();

        return;
    }


    // Timer

    handleTimer();
}


// ====================================================
// EXIT
// ====================================================

bool Pomodoro::wantsToExit()
{
    return _exitRequested;
}


// ====================================================
// MENU
// ====================================================

void Pomodoro::handleMenu()
{
    if (_buttons.leftPressed())
    {
        moveUp();

        _buzzer.beep();

        showMenu();

        return;
    }


    if (_buttons.rightPressed())
    {
        moveDown();

        _buzzer.beep();

        showMenu();

        return;
    }


    if (_buttons.middlePressed())
    {
        selectMenu();

        return;
    }
}


// ====================================================
// MOVE UP
// ====================================================

void Pomodoro::moveUp()
{
    _selection--;

    if (_selection < 0)
    {
        _selection = MENU_COUNT - 1;
    }
}


// ====================================================
// MOVE DOWN
// ====================================================

void Pomodoro::moveDown()
{
    _selection++;

    if (_selection >= MENU_COUNT)
    {
        _selection = 0;
    }
}


// ====================================================
// SELECT MENU
// ====================================================

void Pomodoro::selectMenu()
{
    if (_selection == 0)
    {
        startFocus();

        return;
    }


    if (_selection == 1)
    {
        Serial.println(
            "Pomodoro Settings selected"
        );

        _buzzer.beep();

        return;
    }


    if (_selection == 2)
    {
        // "Back" — tell UI we want to return to the
        // home/menu screen. UI checks this via
        // wantsToExit() right after update() runs.

        _exitRequested = true;

        _buzzer.beep(1500, 150);

        return;
    }
}


// ====================================================
// START FOCUS
// ====================================================

void Pomodoro::startFocus()
{
    _state = FOCUS;

    _session = 1;

    _phaseDuration = _focusDuration;

    _phaseStart = millis();

    _buzzer.beep(2500, 150);

    showTimer();
}


// ====================================================
// START SHORT BREAK
// ====================================================

void Pomodoro::startShortBreak()
{
    _state = SHORT_BREAK;

    _phaseDuration = _shortBreakDuration;

    _phaseStart = millis();

    _buzzer.beep(1800, 150);

    showTimer();
}


// ====================================================
// START LONG BREAK
// ====================================================

void Pomodoro::startLongBreak()
{
    _state = LONG_BREAK;

    _phaseDuration = _longBreakDuration;

    _phaseStart = millis();

    _buzzer.beep(1500, 200);

    showTimer();
}


// ====================================================
// TIMER
// ====================================================

void Pomodoro::handleTimer()
{
    // -----------------------------------------------
    // PAUSED
    // -----------------------------------------------

    if (_state == PAUSED)
    {
        if (_buttons.middlePressed())
        {
            resumeTimer();

            return;
        }


        if (_buttons.rightPressed())
        {
            skipPhase();

            return;
        }


        return;
    }


    // -----------------------------------------------
    // RUNNING
    // -----------------------------------------------

    if (_buttons.middlePressed())
    {
        pauseTimer();

        return;
    }


    if (_buttons.rightPressed())
    {
        skipPhase();

        return;
    }


    // -----------------------------------------------
    // FINISHED
    // -----------------------------------------------

    if (remainingTime() == 0)
    {
        phaseFinished();

        return;
    }


    // -----------------------------------------------
    // REFRESH DISPLAY
    // -----------------------------------------------

    static unsigned long lastDisplayUpdate = 0;

    if (millis() - lastDisplayUpdate >= 1000)
    {
        lastDisplayUpdate = millis();

        showTimer();
    }
}


// ====================================================
// REMAINING TIME
// ====================================================

unsigned long Pomodoro::remainingTime()
{
    if (_state == PAUSED)
    {
        return _pausedRemaining;
    }


    unsigned long elapsed =
        millis() - _phaseStart;


    if (elapsed >= _phaseDuration)
    {
        return 0;
    }


    return _phaseDuration - elapsed;
}


// ====================================================
// PAUSE
// ====================================================

void Pomodoro::pauseTimer()
{
    _pausedRemaining = remainingTime();

    _state = PAUSED;

    _buzzer.beep(2000, 100);

    showTimer();
}


// ====================================================
// RESUME
// ====================================================

void Pomodoro::resumeTimer()
{
    _phaseDuration = _pausedRemaining;

    _phaseStart = millis();

    _state = FOCUS;

    _buzzer.beep(2500, 100);

    showTimer();
}


// ====================================================
// SKIP
// ====================================================

void Pomodoro::skipPhase()
{
    Serial.println(
        "Pomodoro phase skipped"
    );

    phaseFinished();
}


// ====================================================
// PHASE FINISHED
// ====================================================

void Pomodoro::phaseFinished()
{
    _buzzer.beep(3000, 400);


    if (_state == FOCUS)
    {
        if (_session >= TOTAL_SESSIONS)
        {
            startLongBreak();

            return;
        }


        startShortBreak();

        return;
    }


    if (
        _state == SHORT_BREAK ||
        _state == LONG_BREAK
    )
    {
        if (_session < TOTAL_SESSIONS)
        {
            _session++;
        }


        startFocus();

        return;
    }
}


// ====================================================
// TIMER DISPLAY
// ====================================================

void Pomodoro::showTimer()
{
    unsigned long remaining =
        remainingTime();


    unsigned long totalSeconds =
        remaining / 1000;


    unsigned int minutes =
        totalSeconds / 60;


    unsigned int seconds =
        totalSeconds % 60;


    // -----------------------------------------------
    // LEFT OLED — TIMER
    // -----------------------------------------------

    _display.left().clearDisplay();

    _display.left().setTextColor(
        SSD1306_WHITE
    );


    _display.left().setTextSize(1);

    _display.left().setCursor(0, 0);


    if (_state == FOCUS)
    {
        _display.left().println(
            "POMODORO - FOCUS"
        );
    }
    else if (_state == SHORT_BREAK)
    {
        _display.left().println(
            "POMODORO - BREAK"
        );
    }
    else if (_state == LONG_BREAK)
    {
        _display.left().println(
            "POMODORO - LONG"
        );
    }
    else
    {
        _display.left().println(
            "POMODORO - PAUSED"
        );
    }


    _display.left().setTextSize(3);

    _display.left().setCursor(12, 18);


    char timeBuffer[8];

    sprintf(
        timeBuffer,
        "%02u:%02u",
        minutes,
        seconds
    );


    _display.left().println(
        timeBuffer
    );


    _display.left().setTextSize(1);

    _display.left().setCursor(35, 52);

    _display.left().print(
        "Session "
    );

    _display.left().print(
        _session
    );

    _display.left().print(
        "/"
    );

    _display.left().print(
        TOTAL_SESSIONS
    );


    _display.left().display();


    // -----------------------------------------------
    // RIGHT OLED — ENVIRONMENT
    // -----------------------------------------------

    showEnvironment();
}


// ====================================================
// ENVIRONMENT DISPLAY
// ====================================================

void Pomodoro::showEnvironment()
{
    _display.right().clearDisplay();

    _display.right().setTextColor(
        SSD1306_WHITE
    );

    _display.right().setTextSize(1);


    _display.right().setCursor(0, 0);

    _display.right().println(
        "ENVIRONMENT"
    );


    _display.right().drawLine(
        0,
        10,
        127,
        10,
        SSD1306_WHITE
    );


    // Temperature

    _display.right().setCursor(0, 16);

    _display.right().print(
        "Temp: "
    );

    _display.right().print(
        _sensor.temperature(),
        1
    );

    _display.right().println(
        " C"
    );


    // Humidity

    _display.right().setCursor(0, 30);

    _display.right().print(
        "Hum : "
    );

    _display.right().print(
        _sensor.humidity(),
        1
    );

    _display.right().println(
        " %"
    );


    // Pressure

    _display.right().setCursor(0, 44);

    _display.right().print(
        "Pres: "
    );

    _display.right().print(
        _sensor.pressure(),
        1
    );

    _display.right().println(
        " hPa"
    );


    _display.right().display();
}


// ====================================================
// POMODORO MENU
// ====================================================

void Pomodoro::showMenu()
{
    // LEFT OLED

    _display.left().clearDisplay();

    _display.left().setTextColor(
        SSD1306_WHITE
    );

    _display.left().setTextSize(2);

    _display.left().setCursor(0, 0);

    _display.left().println(
        "POMODORO"
    );


    _display.left().setTextSize(1);

    // Draw all menu items from _menuItems[] instead of one
    // hardcoded block per item — this way adding/renaming an
    // item only means editing the array in pomodoro.h, and
    // showMenu() never needs to change again.

    for (int i = 0; i < MENU_COUNT; i++)
    {
        _display.left().setCursor(0, 27 + (i * 13));

        if (_selection == i)
        {
            _display.left().print("> ");
        }
        else
        {
            _display.left().print("  ");
        }

        _display.left().println(_menuItems[i]);
    }


    _display.left().display();


    // RIGHT OLED

    _display.right().clearDisplay();

    _display.right().setTextColor(
        SSD1306_WHITE
    );

    _display.right().setTextSize(1);


    _display.right().setCursor(0, 3);

    _display.right().println(
        "POMODORO MENU"
    );


    _display.right().drawLine(
        0,
        13,
        127,
        13,
        SSD1306_WHITE
    );


    _display.right().setCursor(0, 21);

    _display.right().println(
        "LEFT  - UP"
    );


    _display.right().setCursor(0, 33);

    _display.right().println(
        "RIGHT - DOWN"
    );


    _display.right().setCursor(0, 45);

    _display.right().println(
        "MIDDLE - SELECT"
    );


    _display.right().setCursor(0, 57);

    _display.right().println(
        "SELECT 'BACK'-EXIT"
    );


    _display.right().display();
}