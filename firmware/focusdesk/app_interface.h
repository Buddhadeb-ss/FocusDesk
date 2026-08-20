#ifndef APP_INTERFACE_H
#define APP_INTERFACE_H

// =========================================================
// APP INTERFACE
// =========================================================
//
// Every screen/app that lives inside the MENU (Pomodoro,
// Tasks, Environment, IP/Web, Statistics, Settings...)
// implements this. UI only ever talks to apps through
// these four methods — it never needs to know how a
// specific app draws itself or what buttons do inside it.
//
// To add a new app:
//   1. Create NewApp.h / NewApp.cpp implementing this class
//   2. Add one line to the _apps[] list in FocusDesk.ino
//   3. Add its name to ui.cpp's menu item list
//
// That's it — ui.cpp itself never needs to change again.
// =========================================================

class App
{
public:

    virtual ~App() {}

    // Called once, right when the user opens this app
    // from the menu. Reset any internal state here.
    virtual void begin() = 0;

    // Called every loop() while this app is the active
    // screen. Handle input + redraw here.
    virtual void update() = 0;

    // Return true for one update() cycle when the app
    // wants UI to send it back to HOME (used by apps
    // like Pomodoro that have their own internal "back"
    // logic). Most simple display-only apps can just
    // always return false.
    virtual bool wantsToExit() { return false; }

    // Short display name, e.g. "Tasks", "IP / Web"
    virtual const char* getName() = 0;
};

#endif
