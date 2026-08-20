#ifndef BUTTONS_H
#define BUTTONS_H

class Buttons
{
public:
    void begin();
    void update();

    bool leftPressed();
    bool middlePressed();
    bool rightPressed();

    bool middleHeld();
    bool middleLongHeld();

private:
    bool _leftState = false;
    bool _middleState = false;
    bool _rightState = false;

    bool _leftEvent = false;
    bool _middleEvent = false;
    bool _rightEvent = false;

    bool _middleHoldEvent = false;
    bool _middleLongHoldEvent = false;

    unsigned long _middlePressTime = 0;

    bool _middleHoldTriggered = false;
    bool _middleLongHoldTriggered = false;

    static const unsigned long HOLD_TIME = 750;
    static const unsigned long LONG_HOLD_TIME = 2000;
};

#endif