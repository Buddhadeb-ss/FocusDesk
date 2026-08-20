#include "i2c.h"
#include "config.h"

#include <Wire.h>

void i2cBegin()
{
    // I2C Bus 1
    // OLED 1 + BME280

    Wire.begin(I2C1_SDA, I2C1_SCL);
    Wire.setClock(100000);


    // I2C Bus 2
    // OLED 2

    Wire1.begin(I2C2_SDA, I2C2_SCL);
    Wire1.setClock(100000);
}