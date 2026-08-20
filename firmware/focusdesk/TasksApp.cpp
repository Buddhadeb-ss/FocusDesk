#include "TasksApp.h"


TasksApp::TasksApp(
    Display& display,
    Sensor& sensor,
    WebServerService& webserver
)
    : _display(display),
      _sensor(sensor),
      _webserver(webserver)
{
}


// ====================================================
// BEGIN
// ====================================================

void TasksApp::begin()
{
    _lastUpdate = 0;

    showTasks();
}


// ====================================================
// UPDATE
// ====================================================

void TasksApp::update()
{
    if (millis() - _lastUpdate >= UPDATE_INTERVAL)
    {
        _lastUpdate = millis();

        showTasks();
    }
}


// ====================================================
// NAME
// ====================================================

const char* TasksApp::getName()
{
    return "Tasks";
}


// ====================================================
// TASKS (LEFT OLED)
// ====================================================

void TasksApp::showTasks()
{
    _display.left().clearDisplay();
    _display.left().setTextColor(SSD1306_WHITE);

    _display.left().setTextSize(2);
    _display.left().setCursor(0, 0);
    _display.left().println("TASKS");

    _display.left().drawLine(0, 18, 127, 18, SSD1306_WHITE);

    _display.left().setTextSize(1);

    int count = _webserver.taskCount();

    if (count == 0)
    {
        _display.left().setCursor(0, 32);
        _display.left().println("No tasks");
    }
    else
    {
        // Show up to 3 tasks

        int visible = count;

        if (visible > 3)
        {
            visible = 3;
        }

        for (int i = 0; i < visible; i++)
        {
            _display.left().setCursor(0, 24 + (i * 13));
            _display.left().print(i + 1);
            _display.left().print(". ");

            String task = _webserver.taskAt(i);

            // OLED width protection

            if (task.length() > 19)
            {
                task = task.substring(0, 19);
            }

            _display.left().println(task);
        }
    }

    _display.left().display();

    // Right OLED

    showEnvironment();
}


// ====================================================
// ENVIRONMENT (RIGHT OLED)
// ====================================================

void TasksApp::showEnvironment()
{
    _display.right().clearDisplay();
    _display.right().setTextColor(SSD1306_WHITE);
    _display.right().setTextSize(1);

    _display.right().setCursor(0, 0);
    _display.right().println("ENVIRONMENT");

    _display.right().drawLine(0, 10, 127, 10, SSD1306_WHITE);

    // Temperature

    _display.right().setCursor(0, 16);
    _display.right().print("Temp: ");
    _display.right().print(_sensor.temperature(), 1);
    _display.right().println(" C");

    // Humidity

    _display.right().setCursor(0, 30);
    _display.right().print("Hum : ");
    _display.right().print(_sensor.humidity(), 1);
    _display.right().println(" %");

    // Pressure

    _display.right().setCursor(0, 44);
    _display.right().print("Pres: ");
    _display.right().print(_sensor.pressure(), 1);
    _display.right().println(" hPa");

    _display.right().display();
}
