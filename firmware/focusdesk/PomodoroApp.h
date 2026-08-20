#ifndef POMODORO_APP_H
#define POMODORO_APP_H

#include "app_interface.h"
#include "pomodoro.h"

// =========================================================
// POMODORO APP
// =========================================================
// Thin adapter: makes the existing Pomodoro class fit the
// App interface. Pomodoro's own logic is untouched — this
// file just forwards begin()/update()/wantsToExit() to it.
// =========================================================

class PomodoroApp : public App
{
public:

    PomodoroApp(Pomodoro& pomodoro);

    void begin() override;
    void update() override;
    bool wantsToExit() override;
    const char* getName() override;

private:

    Pomodoro& _pomodoro;
};

#endif
