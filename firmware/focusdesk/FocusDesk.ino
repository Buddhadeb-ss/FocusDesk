#include <Arduino.h>

#include "config.h"
#include "i2c.h"
#include "display.h"
#include "sensor.h"
#include "buttons.h"
#include "buzzer.h"

#include "wifi_service.h"
#include "web_server.h"

#include "pomodoro.h"
#include "PomodoroApp.h"
#include "TasksApp.h"
#include "EnvironmentApp.h"
#include "IPWebApp.h"
#include "PlaceholderApp.h"

#include "app_interface.h"
#include "ui.h"


// ====================================================
// HARDWARE
// ====================================================

Display display;

Sensor sensor;

Buttons buttons;

Buzzer buzzer;


// ====================================================
// SERVICES
// ====================================================

WiFiService wifi;


WebServerService webserver(
    sensor,
    wifi
);


// ====================================================
// APPLICATIONS
// ====================================================
// This is the ONLY place app order/membership is defined.
// To add a new app: create its class, instantiate it here,
// and add it to the apps[] array below (same position =
// same menu slot). ui.cpp never needs to change.
// ====================================================

Pomodoro pomodoro(
    display,
    buttons,
    buzzer,
    sensor
);

PomodoroApp pomodoroApp(pomodoro);

TasksApp tasksApp(
    display,
    sensor,
    webserver
);

EnvironmentApp environmentApp(
    display,
    sensor
);

IPWebApp ipWebApp(
    display,
    wifi
);

PlaceholderApp statisticsApp(
    display,
    buttons,
    buzzer,
    "Statistics"
);

PlaceholderApp settingsApp(
    display,
    buttons,
    buzzer,
    "Settings"
);


App* apps[] =
{
    &pomodoroApp,
    &tasksApp,
    &environmentApp,
    &statisticsApp,
    &ipWebApp,
    &settingsApp
};

const int APP_COUNT = sizeof(apps) / sizeof(apps[0]);


// ====================================================
// UI
// ====================================================

UI ui(
    display,
    buttons,
    sensor,
    buzzer,
    environmentApp,
    apps,
    APP_COUNT
);


// ====================================================
// SETUP
// ====================================================

void setup()
{
    Serial.begin(115200);

    delay(1000);


    Serial.println();

    Serial.println(
        "================================"
    );

    Serial.println(
        "          FOCUSDESK"
    );

    Serial.println(
        "================================"
    );


    // ================================================
    // I2C
    // ================================================

    i2cBegin();


    // ================================================
    // HARDWARE
    // ================================================

    buttons.begin();

    buzzer.begin();


    if (!display.begin())
    {
        Serial.println(
            "DISPLAY FAILED"
        );


        while (true)
        {
            delay(1000);
        }
    }


    if (!sensor.begin())
    {
        Serial.println(
            "BME280 FAILED"
        );
    }


    // ================================================
    // WIFI
    // ================================================

    wifi.begin();


    // ================================================
    // WEB SERVER
    // ================================================

    webserver.begin();


    // ================================================
    // UI
    // ================================================

    ui.begin();


    // ================================================
    // STARTUP BEEP
    // ================================================

    buzzer.beep(
        2000,
        150
    );


    Serial.println(
        "================================"
    );

    Serial.println(
        "       SYSTEM READY"
    );

    Serial.println(
        "================================"
    );
}


// ====================================================
// LOOP
// ====================================================

void loop()
{
    // ================================================
    // HARDWARE
    // ================================================

    buttons.update();

    sensor.update();


    // ================================================
    // SERVICES
    // ================================================

    wifi.update();

    webserver.update();


    // ================================================
    // UI
    // ================================================

    ui.update();
}
