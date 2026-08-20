#ifndef SENSOR_H
#define SENSOR_H

class Sensor
{
public:
    bool begin();
    void update();

    float temperature();
    float humidity();
    float pressure();

private:
    float _temperature = 0;
    float _humidity = 0;
    float _pressure = 0;
};

#endif