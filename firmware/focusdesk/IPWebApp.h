#ifndef IP_WEB_APP_H
#define IP_WEB_APP_H

#include "app_interface.h"
#include "display.h"
#include "wifi_service.h"

// =========================================================
// IP / WEB APP
// =========================================================
// Left OLED  : IP address + WiFi mode
// Right OLED : web server online/offline status
// =========================================================

class IPWebApp : public App
{
public:

    IPWebApp(
        Display& display,
        WiFiService& wifi
    );

    void begin() override;
    void update() override;
    const char* getName() override;

private:

    Display& _display;
    WiFiService& _wifi;

    unsigned long _lastUpdate = 0;
    static const unsigned long UPDATE_INTERVAL = 2000;

    void show();
};

#endif
