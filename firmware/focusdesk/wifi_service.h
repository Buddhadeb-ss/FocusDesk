#ifndef WIFI_SERVICE_H
#define WIFI_SERVICE_H

#include <Arduino.h>
#include <WiFi.h>

class WiFiService
{
public:
    void begin();
    void update();

    bool connected();

    String ipAddress();
    String mode();

private:
    bool _connected = false;
};

#endif