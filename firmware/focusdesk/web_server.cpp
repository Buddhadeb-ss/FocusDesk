#include "web_server.h"

#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>


// ====================================================
// WEB SERVER
// ====================================================

static WebServer server(80);

static Preferences preferences;


// ====================================================
// TASK STORAGE
// ====================================================

static const int MAX_TASKS = 10;

static String tasks[MAX_TASKS];

static int _taskCount = 0;


// ====================================================
// LOAD TASKS
// ====================================================

static void loadTasks()
{
    preferences.begin(
        "focusdesk",
        false
    );


    _taskCount =
        preferences.getInt(
            "taskCount",
            0
        );


    if (_taskCount < 0)
    {
        _taskCount = 0;
    }


    if (_taskCount > MAX_TASKS)
    {
        _taskCount = MAX_TASKS;
    }


    for (
        int i = 0;
        i < _taskCount;
        i++
    )
    {
        String key =
            "task" + String(i);


        tasks[i] =
            preferences.getString(
                key.c_str(),
                ""
            );
    }
}


// ====================================================
// SAVE TASKS
// ====================================================

static void saveTasks()
{
    preferences.putInt(
        "taskCount",
        _taskCount
    );


    for (
        int i = 0;
        i < MAX_TASKS;
        i++
    )
    {
        String key =
            "task" + String(i);


        if (i < _taskCount)
        {
            preferences.putString(
                key.c_str(),
                tasks[i]
            );
        }
        else
        {
            preferences.remove(
                key.c_str()
            );
        }
    }
}


// ====================================================
// CONSTRUCTOR
// ====================================================

WebServerService::WebServerService(
    Sensor& sensor,
    WiFiService& wifi
)
    : _sensor(sensor),
      _wifi(wifi)
{
}


// ====================================================
// BEGIN
// ====================================================

void WebServerService::begin()
{
    loadTasks();


    // ================================================
    // HOME PAGE
    // ================================================

    server.on(
        "/",
        HTTP_GET,
        [this]()
        {
            handleRoot();
        }
    );


    // ================================================
    // ENVIRONMENT API
    // ================================================

    server.on(
        "/api/env",
        HTTP_GET,
        [this]()
        {
            handleEnvironment();
        }
    );


    // ================================================
    // TASK API
    // ================================================

    server.on(
        "/api/tasks",
        HTTP_GET,
        [this]()
        {
            handleTasks();
        }
    );


    // ================================================
    // ADD TASK
    // ================================================

    server.on(
        "/api/tasks/add",
        HTTP_POST,
        []()
        {
            if (!server.hasArg("title"))
            {
                server.send(
                    400,
                    "application/json",
                    "{\"error\":\"missing title\"}"
                );

                return;
            }


            if (_taskCount >= MAX_TASKS)
            {
                server.send(
                    400,
                    "application/json",
                    "{\"error\":\"task limit reached\"}"
                );

                return;
            }


            String title =
                server.arg("title");


            title.trim();


            if (title.length() == 0)
            {
                server.send(
                    400,
                    "application/json",
                    "{\"error\":\"empty title\"}"
                );

                return;
            }


            tasks[_taskCount] =
                title.substring(0, 60);


            _taskCount++;


            saveTasks();


            server.send(
                200,
                "application/json",
                "{\"ok\":true}"
            );
        }
    );


    // ================================================
    // DELETE TASK
    // ================================================

    server.on(
        "/api/tasks/delete",
        HTTP_POST,
        []()
        {
            if (!server.hasArg("id"))
            {
                server.send(
                    400,
                    "application/json",
                    "{\"error\":\"missing id\"}"
                );

                return;
            }


            int id =
                server.arg("id").toInt();


            if (
                id < 0 ||
                id >= _taskCount
            )
            {
                server.send(
                    400,
                    "application/json",
                    "{\"error\":\"invalid id\"}"
                );

                return;
            }


            for (
                int i = id;
                i < _taskCount - 1;
                i++
            )
            {
                tasks[i] =
                    tasks[i + 1];
            }


            _taskCount--;


            tasks[_taskCount] = "";


            saveTasks();


            server.send(
                200,
                "application/json",
                "{\"ok\":true}"
            );
        }
    );


    // ================================================
    // NOT FOUND
    // ================================================

    server.onNotFound(
        [this]()
        {
            handleNotFound();
        }
    );


    // ================================================
    // START SERVER
    // ================================================

    server.begin();


    Serial.println(
        "Web server started"
    );


    Serial.print(
        "Open: http://"
    );


    Serial.println(
        _wifi.ipAddress()
    );
}


// ====================================================
// UPDATE
// ====================================================

void WebServerService::update()
{
    server.handleClient();
}


// ====================================================
// TASK COUNT
// ====================================================

int WebServerService::taskCount()
{
    return _taskCount;
}


// ====================================================
// TASK AT INDEX
// ====================================================

String WebServerService::taskAt(
    int index
)
{
    if (
        index < 0 ||
        index >= _taskCount
    )
    {
        return "";
    }


    return tasks[index];
}


// ====================================================
// ROOT
// ====================================================

void WebServerService::handleRoot()
{
    server.send(
        200,
        "text/html",
        buildPage()
    );
}


// ====================================================
// ENVIRONMENT
// ====================================================

void WebServerService::handleEnvironment()
{
    server.send(
        200,
        "application/json",
        buildEnvironmentJSON()
    );
}


// ====================================================
// TASKS
// ====================================================

void WebServerService::handleTasks()
{
    server.send(
        200,
        "application/json",
        buildTasksJSON()
    );
}


// ====================================================
// ENVIRONMENT JSON
// ====================================================

String WebServerService::buildEnvironmentJSON()
{
    String json = "{";


    json += "\"temperature\":";
    json += String(
        _sensor.temperature(),
        1
    );


    json += ",";


    json += "\"humidity\":";
    json += String(
        _sensor.humidity(),
        1
    );


    json += ",";


    json += "\"pressure\":";
    json += String(
        _sensor.pressure(),
        1
    );


    json += "}";


    return json;
}


// ====================================================
// TASK JSON
// ====================================================

String WebServerService::buildTasksJSON()
{
    String json = "[";


    for (
        int i = 0;
        i < _taskCount;
        i++
    )
    {
        if (i > 0)
        {
            json += ",";
        }


        json += "{";


        json += "\"id\":";
        json += String(i);


        json += ",";


        json += "\"title\":\"";


        String title =
            tasks[i];


        title.replace(
            "\\",
            "\\\\"
        );


        title.replace(
            "\"",
            "\\\""
        );


        json += title;


        json += "\"";


        json += "}";
    }


    json += "]";


    return json;
}


// ====================================================
// WEB PAGE
// ====================================================

String WebServerService::buildPage()
{
    String html = R"HTMLPAGE(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>FocusDesk</title>
<style>

:root {
    --bg: #0b0d10;
    --surface: #15181c;
    --surface-2: #1d2126;
    --border: #262b31;
    --text: #eef1f4;
    --muted: #8b939c;
    --accent: #5ee0a0;
    --accent-dim: rgba(94, 224, 160, 0.15);
    --danger: #e05e5e;
    --radius: 14px;
}

* {
    box-sizing: border-box;
}

body {
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Arial, sans-serif;
    background: var(--bg);
    color: var(--text);
    margin: 0;
    padding: 24px 16px 60px;
    min-height: 100vh;
}

.wrap {
    max-width: 760px;
    margin: 0 auto;
}

.top {
    display: flex;
    align-items: center;
    justify-content: space-between;
    margin-bottom: 22px;
    flex-wrap: wrap;
    gap: 10px;
}

.brand {
    display: flex;
    align-items: center;
    gap: 10px;
}

.brand h1 {
    font-size: 20px;
    letter-spacing: 0.5px;
    margin: 0;
    font-weight: 700;
}

.dot {
    width: 9px;
    height: 9px;
    border-radius: 50%;
    background: var(--accent);
    box-shadow: 0 0 8px var(--accent);
    flex-shrink: 0;
}

.status-pill {
    font-size: 12px;
    color: var(--muted);
    background: var(--surface);
    border: 1px solid var(--border);
    padding: 6px 12px;
    border-radius: 999px;
}

.card {
    background: var(--surface);
    border: 1px solid var(--border);
    border-radius: var(--radius);
    padding: 20px;
    margin-bottom: 16px;
}

.card h2 {
    font-size: 13px;
    text-transform: uppercase;
    letter-spacing: 1px;
    color: var(--muted);
    margin: 0 0 16px;
    font-weight: 600;
}

.grid3 {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    gap: 10px;
}

.metric {
    background: var(--surface-2);
    border: 1px solid var(--border);
    border-radius: 10px;
    padding: 16px 10px;
    text-align: center;
    transition: border-color 0.2s ease;
}

.metric .num {
    font-size: 26px;
    font-weight: 700;
    font-variant-numeric: tabular-nums;
    color: var(--accent);
    line-height: 1.1;
}

.metric .unit {
    font-size: 11px;
    color: var(--muted);
    margin-top: 4px;
    text-transform: uppercase;
    letter-spacing: 0.5px;
}

.task-form {
    display: flex;
    gap: 8px;
    margin-bottom: 4px;
}

.task-form input {
    flex: 1;
    padding: 12px 14px;
    border-radius: 10px;
    border: 1px solid var(--border);
    background: var(--surface-2);
    color: var(--text);
    font-size: 14px;
    outline: none;
    transition: border-color 0.2s ease;
}

.task-form input:focus {
    border-color: var(--accent);
}

.btn {
    padding: 12px 18px;
    border-radius: 10px;
    border: none;
    background: var(--accent);
    color: #06130d;
    font-weight: 700;
    font-size: 14px;
    cursor: pointer;
    transition: opacity 0.15s ease, transform 0.1s ease;
}

.btn:hover {
    opacity: 0.9;
}

.btn:active {
    transform: scale(0.97);
}

.task-list {
    margin-top: 14px;
    display: flex;
    flex-direction: column;
    gap: 8px;
}

.task-row {
    display: flex;
    align-items: center;
    gap: 10px;
    background: var(--surface-2);
    border: 1px solid var(--border);
    border-radius: 10px;
    padding: 10px 12px;
    animation: fadeIn 0.2s ease;
}

.task-row .title {
    flex: 1;
    font-size: 14px;
    word-break: break-word;
}

.task-row button {
    background: transparent;
    border: 1px solid var(--border);
    color: var(--danger);
    border-radius: 8px;
    padding: 6px 10px;
    font-size: 12px;
    cursor: pointer;
    transition: background 0.15s ease;
}

.task-row button:hover {
    background: rgba(224, 94, 94, 0.1);
}

.empty {
    color: var(--muted);
    font-size: 13px;
    padding: 10px 2px;
}

@keyframes fadeIn {
    from { opacity: 0; transform: translateY(-4px); }
    to { opacity: 1; transform: translateY(0); }
}

@media (max-width: 480px) {
    .grid3 {
        grid-template-columns: repeat(3, 1fr);
        gap: 6px;
    }

    .metric {
        padding: 12px 6px;
    }

    .metric .num {
        font-size: 20px;
    }

    .task-form {
        flex-direction: column;
    }

    .btn {
        width: 100%;
    }
}

</style>
</head>
<body>

<div class="wrap">

  <div class="top">
    <div class="brand">
      <span class="dot"></span>
      <h1>FOCUSDESK</h1>
    </div>
    <div class="status-pill" id="statusPill">connecting...</div>
  </div>

  <div class="card">
    <h2>Environment</h2>
    <div class="grid3">
      <div class="metric">
        <div class="num" id="temperature">--</div>
        <div class="unit">deg C</div>
      </div>
      <div class="metric">
        <div class="num" id="humidity">--</div>
        <div class="unit">% RH</div>
      </div>
      <div class="metric">
        <div class="num" id="pressure">--</div>
        <div class="unit">hPa</div>
      </div>
    </div>
  </div>

  <div class="card">
    <h2>Tasks</h2>

    <div class="task-form">
      <input id="taskInput" placeholder="Add a task and press Enter" maxlength="60">
      <button class="btn" onclick="addTask()">Add</button>
    </div>

    <div class="task-list" id="tasks">
      <div class="empty">Loading...</div>
    </div>
  </div>

</div>

<script>

const el = (id) => document.getElementById(id);

let envFailCount = 0;

async function updateEnvironment() {
    try {
        const res = await fetch('/api/env');
        const data = await res.json();

        el('temperature').textContent = data.temperature.toFixed(1);
        el('humidity').textContent = data.humidity.toFixed(1);
        el('pressure').textContent = data.pressure.toFixed(0);

        envFailCount = 0;
        el('statusPill').textContent = 'online';
    } catch (e) {
        envFailCount++;
        if (envFailCount >= 2) {
            el('statusPill').textContent = 'offline';
        }
    }
}

async function updateTasks() {
    try {
        const res = await fetch('/api/tasks');
        const list = await res.json();

        const container = el('tasks');

        if (list.length === 0) {
            container.innerHTML = '<div class="empty">No tasks yet — add one above.</div>';
            return;
        }

        container.innerHTML = list.map(t =>
            '<div class="task-row">' +
                '<span class="title">' + escapeHtml(t.title) + '</span>' +
                '<button onclick="deleteTask(' + t.id + ')">Delete</button>' +
            '</div>'
        ).join('');
    } catch (e) {
        console.log(e);
    }
}

function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
}

async function addTask() {
    const input = el('taskInput');
    const title = input.value.trim();

    if (!title) {
        return;
    }

    const body = new URLSearchParams();
    body.append('title', title);

    await fetch('/api/tasks/add', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: body
    });

    input.value = '';
    updateTasks();
}

async function deleteTask(id) {
    const body = new URLSearchParams();
    body.append('id', id);

    await fetch('/api/tasks/delete', {
        method: 'POST',
        headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
        body: body
    });

    updateTasks();
}

el('taskInput').addEventListener('keydown', (e) => {
    if (e.key === 'Enter') {
        addTask();
    }
});

updateEnvironment();
updateTasks();

setInterval(updateEnvironment, 1000);
setInterval(updateTasks, 3000);

</script>
</body>
</html>
)HTMLPAGE";

    return html;
}


// ====================================================
// NOT FOUND
// ====================================================

void WebServerService::handleNotFound()
{
    server.send(
        404,
        "text/plain",
        "FocusDesk: Not Found"
    );
}