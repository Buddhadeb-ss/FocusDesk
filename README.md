# FocusDesk

A deadline clock designed for students and professional users to keep track of time and manage deadlines.

Built on an ESP32 with dual OLED displays, a BME280 environment sensor, and a WiFi-connected web dashboard for task management.

---

## Status

Hackathon-stage project. Core navigation, Pomodoro timer, task list, environment monitoring, and web dashboard are functional. Statistics and Settings are currently placeholder screens. This repo is a snapshot — not actively being extended further at this time, but kept as a reference for the architecture lesson below and as a base to pick back up later.

---

## What it does

- **Pomodoro timer** — focus/short-break/long-break cycle with session tracking, pause/resume, skip, and an in-app Back option
- **Task list** — add/view/delete tasks, synced between the device and the web dashboard, persisted to flash
- **Environment monitoring** — live temperature, humidity, and pressure from a BME280, shown both as raw numbers (web dashboard, home screen) and as plain-language status ("Comfortable", "High Humidity") on the device's own environment screen
- **Web dashboard** — the ESP32 hosts its own responsive local web page for adding/removing tasks and watching environment data update in real time, no app or cloud required
- **IP/Web screen** — shows the device's IP and WiFi status directly on the OLED so you know where to point a browser

Navigation is button-driven: two OLEDs (one for content, one for controls/status), three buttons (left/right/middle), with a menu you scroll through to open each app.

---

## Repo structure

```
firmware/          ESP32 source — FocusDesk.ino + all .cpp/.h modules
hardware/           Schematics / wiring / BOM
web-dashboard/      (see note below)
docs/               Notes, writeups
images/             Photos, screenshots, renders
```

**Note on `web-dashboard/`:** the dashboard's HTML/CSS/JS currently lives as a single string served directly from `firmware/web_server.cpp`, not as standalone files in this folder. If picking this project back up, pulling that out into real `.html`/`.css`/`.js` files served via `SPIFFS`/`LittleFS` would be a natural next step — it's on the list of architectural improvements below.

---

## Firmware architecture

The firmware went through one real architectural rewrite worth documenting, because it's the actual lesson from this project.

**What went wrong initially:** every app/screen (Pomodoro, Tasks, Environment, IP/Web, Statistics, Settings) had its rendering and logic written directly inside one class, `UI`, in a single file. That file grew to **1300+ lines**. Past a certain point, changing one app risked breaking an unrelated one, since everything shared the same class and the same file — there was no real separation, just different `if` blocks next to each other.

**The fix:** an `App` interface (`app_interface.h`) — every screen implements `begin()`, `update()`, `wantsToExit()`, `getName()`. Each app became its own class in its own file (`TasksApp.cpp`, `EnvironmentApp.cpp`, etc.). `UI` was rewritten to hold only navigation state (home/menu/app screen switching) and an array of `App*` — it no longer knows *how* any individual app renders itself, only how to call the four interface methods.

Result: `ui.cpp` dropped from ~1300 lines to ~250, and adding a new app now means creating one file pair and adding one line to the array in `FocusDesk.ino` — nothing else changes.

```
firmware/
├── app_interface.h                                    Common interface every app implements
├── PomodoroApp.*                                      Wraps the standalone Pomodoro class
├── TasksApp.*                                         Task list screen
├── EnvironmentApp.*                                   Temp/humidity/pressure screen + shared readout
├── IPWebApp.*                                         IP address / web server status screen
├── PlaceholderApp.*                                   Reusable "coming soon" screen (Statistics, Settings)
├── ui.*                                               Navigation only: home/menu state machine
├── pomodoro.*                                         Core Pomodoro timer logic (pre-existing, unchanged)
├── display.*, buttons.*, buzzer.*, sensor.*, i2c.*    Hardware drivers
├── wifi_service.*, web_server.*                       WiFi + local dashboard
└── FocusDesk.ino                                      Wiring: instantiates every app, builds the App* array
```

### Known bugs fixed along the way

- **Menu navigation could get stuck inside an app.** Root cause: an exit flag (`_exitRequested` in `Pomodoro`) was being reset to `false` at the top of every single update cycle, before anything could ever act on it — so it could never actually signal "go back." Fixed by only resetting it when the app opens, and adding an explicit "Back" menu option that sets it correctly.
- A related bug was caught while fixing the above: the "Back" option existed in data (`_menuItems[]`) but the on-screen menu renderer had hardcoded drawing for only 2 items, so "Back" would have been selectable but invisible. Fixed by driving the renderer off the same array instead of hardcoding each item.

If picking this project back up, this — reading the actual render/update code before trusting that "the data model says X" means "the UI shows X" — is the main debugging habit worth carrying forward.

---

## Hardware

See `hardware/` for schematics and wiring. Built around:
- ESP32 (WiFi + dual-core)
- 2x SSD1306 OLED (I2C)
- BME280 (temperature / humidity / pressure, I2C)
- 3 push buttons (left / middle / right)
- Piezo buzzer

---

## Lessons for the next project

- Design the module boundary (one file/class per feature) **before** writing the second feature, not after the third one makes the first file unmanageable
- A shared interface (even a tiny one — 3–4 methods) is worth defining early if more than ~2 similar "screens/modes/apps" are planned from the start
- Don't trust that a data structure (like a menu items array) is actually driving what's on screen — check the render function directly

---

## License

Apache-2.0 — see `LICENSE`.
