#include "PomodoroApp.h"


PomodoroApp::PomodoroApp(Pomodoro& pomodoro)
    : _pomodoro(pomodoro)
{
}


void PomodoroApp::begin()
{
    _pomodoro.begin();
}


void PomodoroApp::update()
{
    _pomodoro.update();
}


bool PomodoroApp::wantsToExit()
{
    return _pomodoro.wantsToExit();
}


const char* PomodoroApp::getName()
{
    return "Pomodoro";
}
