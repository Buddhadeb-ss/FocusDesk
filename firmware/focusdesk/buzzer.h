#ifndef BUZZER_H
#define BUZZER_H

class Buzzer
{
public:
    void begin();

    void beep();
    void beep(unsigned int frequency, unsigned long duration);
};

#endif