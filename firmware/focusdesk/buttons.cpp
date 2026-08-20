#include "buttons.h"
#include "config.h"

#include <Arduino.h>


void Buttons::begin()
{
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_MIDDLE, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
}


void Buttons::update()
{
    bool leftNow = !digitalRead(BTN_LEFT);
    bool middleNow = !digitalRead(BTN_MIDDLE);
    bool rightNow = !digitalRead(BTN_RIGHT);


    // Reset events every update

    _leftEvent = false;
    _middleEvent = false;
    _rightEvent = false;

    _middleHoldEvent = false;
    _middleLongHoldEvent = false;


    // =================================================
    // LEFT
    // =================================================

    if (leftNow && !_leftState)
    {
        _leftEvent = true;
    }


    // =================================================
    // RIGHT
    // =================================================

    if (rightNow && !_rightState)
    {
        _rightEvent = true;
    }


    // =================================================
    // MIDDLE PRESS
    // =================================================

    if (middleNow && !_middleState)
    {
        _middleEvent = true;

        _middlePressTime = millis();

        _middleHoldTriggered = false;
        _middleLongHoldTriggered = false;
    }


    // =================================================
    // MIDDLE HOLD — 750 ms
    // =================================================

    if (middleNow &&
        !_middleHoldTriggered &&
        (millis() - _middlePressTime >= HOLD_TIME))
    {
        _middleHoldEvent = true;

        _middleHoldTriggered = true;
    }


    // =================================================
    // MIDDLE LONG HOLD — 2 seconds
    // =================================================

    if (middleNow &&
        !_middleLongHoldTriggered &&
        (millis() - _middlePressTime >= LONG_HOLD_TIME))
    {
        _middleLongHoldEvent = true;

        _middleLongHoldTriggered = true;
    }


    // =================================================
    // MIDDLE RELEASE
    // =================================================

    if (!middleNow && _middleState)
    {
        _middleHoldTriggered = false;
        _middleLongHoldTriggered = false;
    }


    // Save current states

    _leftState = leftNow;
    _middleState = middleNow;
    _rightState = rightNow;
}


bool Buttons::leftPressed()
{
    return _leftEvent;
}


bool Buttons::middlePressed()
{
    return _middleEvent;
}


bool Buttons::rightPressed()
{
    return _rightEvent;
}


bool Buttons::middleHeld()
{
    return _middleHoldEvent;
}


bool Buttons::middleLongHeld()
{
    return _middleLongHoldEvent;
}