#ifndef PLACEHOLDER_APP_H
#define PLACEHOLDER_APP_H

#include "app_interface.h"
#include "display.h"
#include "buttons.h"
#include "buzzer.h"

// =========================================================
// PLACEHOLDER APP
// =========================================================
// Reused for any menu slot that doesn't have a real app
// yet (currently: Statistics, Settings). Give it the name
// you want shown, and it beeps on any button press so it
// still feels responsive. Once you build the real app,
// just swap this instance out for the real one in the .ino
// — nothing else needs to change.
// =========================================================

class PlaceholderApp : public App
{
public:

    PlaceholderApp(
        Display& display,
        Buttons& buttons,
        Buzzer& buzzer,
        const char* name
    );

    void begin() override;
    void update() override;
    const char* getName() override;

private:

    Display& _display;
    Buttons& _buttons;
    Buzzer& _buzzer;
    const char* _name;

    void show();
};

#endif
