#include "sensor.h"
#include "config.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

static Adafruit_BME280 bme;

bool Sensor::begin()
{
    Serial.println("Initializing BME280...");

    if (!bme.begin(0x76, &Wire))
    {
        Serial.println("BME280 FAILED");
        return false;
    }

    Serial.println("BME280 OK");

    return true;
}

void Sensor::update()
{
    _temperature = bme.readTemperature();
    _humidity = bme.readHumidity();
    _pressure = bme.readPressure() / 100.0F;
}

float Sensor::temperature()
{
    return _temperature;
}

float Sensor::humidity()
{
    return _humidity;
}

float Sensor::pressure()
{
    return _pressure;
}