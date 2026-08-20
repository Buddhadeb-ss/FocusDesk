#ifndef WEBSERVER_SERVICE_H
#define WEBSERVER_SERVICE_H

#include <Arduino.h>
#include <WebServer.h>

#include "sensor.h"
#include "wifi_service.h"

class WebServerService
{
public:

    WebServerService(
        Sensor& sensor,
        WiFiService& wifi
    );

    void begin();
    void update();


    // ================================================
    // TASK DATA
    // ================================================

    int taskCount();

    String taskAt(int index);


private:

    Sensor& _sensor;

    WiFiService& _wifi;


    // ================================================
    // WEB HANDLERS
    // ================================================

    void handleRoot();

    void handleEnvironment();

    void handleTasks();

    void handleNotFound();


    // ================================================
    // PAGE / API
    // ================================================

    String buildPage();

    String buildEnvironmentJSON();

    String buildTasksJSON();
};

#endif