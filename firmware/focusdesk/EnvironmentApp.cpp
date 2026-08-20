#include "EnvironmentApp.h"


EnvironmentApp::EnvironmentApp(
    Display& display,
    Sensor& sensor
)
    : _display(display),
      _sensor(sensor)
{
}


// ====================================================
// BEGIN
// ====================================================

void EnvironmentApp::begin()
{
    _lastUpdate = 0;

    showLeftDetail();
    showReadout();
}


// ====================================================
// UPDATE
// ====================================================

void EnvironmentApp::update()
{
    if (millis() - _lastUpdate >= UPDATE_INTERVAL)
    {
        _lastUpdate = millis();

        showLeftDetail();
        showReadout();
    }
}


// ====================================================
// NAME
// ====================================================

const char* EnvironmentApp::getName()
{
    return "Environment";
}


// ====================================================
// RIGHT OLED — shared readout
// ====================================================
// This is the exact drawing code that used to be
// duplicated in ui.cpp's showHome() and showTasks().
// Now it lives in exactly one place.
// ====================================================

void EnvironmentApp::showReadout()
{
    _display.right().clearDisplay();
    _display.right().setTextColor(SSD1306_WHITE);
    _display.right().setTextSize(1);

    _display.right().setCursor(0, 0);
    _display.right().println("ENVIRONMENT");

    _display.right().drawLine(0, 10, 127, 10, SSD1306_WHITE);

    _display.right().setCursor(0, 16);
    _display.right().print("Temp: ");
    _display.right().print(_sensor.temperature(), 1);
    _display.right().println(" C");

    _display.right().setCursor(0, 30);
    _display.right().print("Hum : ");
    _display.right().print(_sensor.humidity(), 1);
    _display.right().println(" %");

    _display.right().setCursor(0, 44);
    _display.right().print("Pres: ");
    _display.right().print(_sensor.pressure(), 1);
    _display.right().println(" hPa");

    _display.right().display();
}


// ====================================================
// LEFT OLED — dedicated detail view for this app
// ====================================================
// This is the Environment app's own screen, so it shows
// descriptive statements instead of raw numbers. The
// shared readout on the right OLED (showReadout(), also
// used by HOME) is left as numeric — that one is a raw
// data display other screens depend on, not this app's
// own interpretation of the data.
// ====================================================

void EnvironmentApp::showLeftDetail()
{
    _display.left().clearDisplay();
    _display.left().setTextColor(SSD1306_WHITE);

    _display.left().setTextSize(2);
    _display.left().setCursor(0, 0);
    _display.left().println("CLIMATE");

    _display.left().drawLine(0, 18, 127, 18, SSD1306_WHITE);

    _display.left().setTextSize(1);

    _display.left().setCursor(0, 26);
    _display.left().print("Temp: ");
    _display.left().println(temperatureLabel(_sensor.temperature()));

    _display.left().setCursor(0, 40);
    _display.left().print("Humid: ");
    _display.left().println(humidityLabel(_sensor.humidity()));

    _display.left().setCursor(0, 54);
    _display.left().print("Press: ");
    _display.left().println(pressureLabel(_sensor.pressure()));

    _display.left().display();
}


// ====================================================
// THRESHOLD LABELS
// ====================================================
// Bands are a reasonable starting point for an indoor
// desk environment — adjust the numbers below to taste.
// ====================================================

const char* EnvironmentApp::temperatureLabel(float tempC)
{
    if (tempC < 18.0)
    {
        return "Cold";
    }

    if (tempC < 24.0)
    {
        return "Comfortable";
    }

    if (tempC < 28.0)
    {
        return "Warm";
    }

    return "Too Hot";
}


const char* EnvironmentApp::humidityLabel(float humidityPct)
{
    if (humidityPct < 30.0)
    {
        return "Too Dry";
    }

    if (humidityPct < 60.0)
    {
        return "Comfortable";
    }

    if (humidityPct < 70.0)
    {
        return "High Humidity";
    }

    return "Very Humid";
}


const char* EnvironmentApp::pressureLabel(float pressureHPa)
{
    if (pressureHPa < 1000.0)
    {
        return "Low";
    }

    if (pressureHPa < 1020.0)
    {
        return "Normal";
    }

    return "High";
}
