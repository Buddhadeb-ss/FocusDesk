#include "wifi_service.h"

#include <WiFi.h>

#include "config.h"


void WiFiService::begin()
{
    Serial.println();
    Serial.println("==============================");
    Serial.println("Starting WiFi...");
    Serial.println("==============================");


    // ------------------------------------------------
    // Try normal WiFi if credentials exist
    // ------------------------------------------------

#ifdef WIFI_SSID

    WiFi.mode(WIFI_STA);

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );


    Serial.print("Connecting");

    unsigned long startTime = millis();


    while (
        WiFi.status() != WL_CONNECTED &&
        millis() - startTime < 10000
    )
    {
        delay(250);

        Serial.print(".");
    }


    Serial.println();


    if (WiFi.status() == WL_CONNECTED)
    {
        _connected = true;

        Serial.println("WiFi connected");

        Serial.print("IP: ");
        Serial.println(WiFi.localIP());

        return;
    }

#endif


    // ------------------------------------------------
    // Fallback Access Point
    // ------------------------------------------------

    Serial.println(
        "Starting FocusDesk Access Point..."
    );


    WiFi.mode(WIFI_AP);

    WiFi.softAP(
        "FocusDesk",
        "focusdesk"
    );


    _connected = true;


    Serial.print("AP IP: ");

    Serial.println(
        WiFi.softAPIP()
    );
}


// ====================================================
// UPDATE
// ====================================================

void WiFiService::update()
{
    // Nothing blocking here.

    // Connection monitoring will be expanded later.
}


// ====================================================
// CONNECTED
// ====================================================

bool WiFiService::connected()
{
    return _connected;
}


// ====================================================
// IP ADDRESS
// ====================================================

String WiFiService::ipAddress()
{
    if (WiFi.getMode() == WIFI_AP)
    {
        return WiFi.softAPIP().toString();
    }


    return WiFi.localIP().toString();
}


// ====================================================
// MODE
// ====================================================

String WiFiService::mode()
{
    if (WiFi.getMode() == WIFI_AP)
    {
        return "AP";
    }


    return "WiFi";
}