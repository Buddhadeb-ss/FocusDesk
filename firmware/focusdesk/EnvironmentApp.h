#ifndef ENVIRONMENT_APP_H
#define ENVIRONMENT_APP_H

#include "app_interface.h"
#include "display.h"
#include "sensor.h"

// =========================================================
// ENVIRONMENT APP
// =========================================================
// Single source of truth for drawing temp/humidity/pressure
// to the right OLED. Both the HOME screen and TasksApp show
// this same data — instead of copy-pasting the drawing code
// again, they call EnvironmentApp::showReadout() directly.
// =========================================================

class EnvironmentApp : public App
{
public:

    EnvironmentApp(
        Display& display,
        Sensor& sensor
    );

    void begin() override;
    void update() override;
    const char* getName() override;

    // Public so HOME (and any other screen) can reuse the
    // exact same right-OLED readout without duplicating it.
    void showReadout();

private:

    Display& _display;
    Sensor& _sensor;

    unsigned long _lastUpdate = 0;
    static const unsigned long UPDATE_INTERVAL = 1000;

    void showLeftDetail();

    // Threshold-based descriptive labels. Kept as small pure
    // functions so the ranges are easy to find and tune later
    // — just edit the numbers in EnvironmentApp.cpp, nothing
    // else needs to change.

    const char* temperatureLabel(float tempC);
    const char* humidityLabel(float humidityPct);
    const char* pressureLabel(float pressureHPa);
};

#endif
