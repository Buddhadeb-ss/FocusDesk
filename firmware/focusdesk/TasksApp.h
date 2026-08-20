#ifndef TASKS_APP_H
#define TASKS_APP_H

#include "app_interface.h"
#include "display.h"
#include "sensor.h"
#include "web_server.h"

// =========================================================
// TASKS APP
// =========================================================
// Left OLED  : task list pulled from the web server
// Right OLED : environment readout (temp/hum/pressure)
// =========================================================

class TasksApp : public App
{
public:

    TasksApp(
        Display& display,
        Sensor& sensor,
        WebServerService& webserver
    );

    void begin() override;
    void update() override;
    const char* getName() override;

private:

    Display& _display;
    Sensor& _sensor;
    WebServerService& _webserver;

    unsigned long _lastUpdate = 0;
    static const unsigned long UPDATE_INTERVAL = 2000;

    void showTasks();
    void showEnvironment();
};

#endif
