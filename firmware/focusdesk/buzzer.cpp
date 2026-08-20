#include "buzzer.h"
#include "config.h"
#include <Arduino.h>

void Buzzer::begin()
{
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
}

void Buzzer::beep()
{
    tone(BUZZER_PIN, 2000, 100);
}

void Buzzer::beep(unsigned int frequency, unsigned long duration)
{
    tone(BUZZER_PIN, frequency, duration);
}