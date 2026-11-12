# MecLab Traffic Light

An interactive traffic-light simulation that runs in the browser and was also built as a physical demo: an **ESP8266** board hosted the web page and drove three real LEDs (red, yellow, green) as a working traffic light.

The web part is plain HTML/CSS/JavaScript and works on its own, **no hardware needed**.

## Features

* Animated road scene with a looping background video
* Clickable traffic light: red, yellow (blinking) and green
* Realistic sequencing: red → red+yellow → green, and green → yellow → red
* Cars drive away down the road when the light turns green
* ESP8266 firmware that serves the page and exposes simple HTTP endpoints to switch the LEDs

## Run it without hardware

1. Open `firmware/traffic\_light\_esp8266/data/index.html` in any modern browser.
2. Click the lights to change state:

   * **Red**: stop (cars wait)
   * **Yellow**: blinking warning
   * **Green**: go (cars drive off)

## Run it on an ESP8266

The firmware serves the web files from the board's flash (SPIFFS) and controls the LEDs.

**Hardware**

|Colour|GPIO|NodeMCU / D1 mini pin|
|-|-|-|
|Red|0|D3|
|Green|2|D4|
|Yellow|4|D2|

The LEDs are **active-low** in the code (`LOW` = on, `HIGH` = off). Use a current-limiting resistor (about 220 Ω) per LED.

**Setup**

1. Install the ESP8266 board package in the Arduino IDE.
2. In `firmware/traffic\_light\_esp8266/`, copy `secrets.h.example` to `secrets.h` and enter your Wi-Fi name and password.
3. Select your ESP8266 board and a flash size that includes a filesystem (at least 2 MB, with 1 MB or more for SPIFFS).
4. Upload the sketch `traffic\_light\_esp8266.ino`.
5. Upload the contents of the `data/` folder to the board's filesystem using the ESP8266 sketch data upload tool.
6. Open the Serial Monitor (115200 baud) to see the board's IP address, then visit it in a browser.

**HTTP endpoints**

|Endpoint|Action|
|-|-|
|`/`|Serves the web interface|
|`/redon`, `/redoff`|Red LED on / off|
|`/greenon`, `/greenoff`|Green LED on / off|
|`/yellowon`, `/yellowoff`|Yellow LED on / off|
|`/yellowblink`|Yellow LED blinks every 500 ms|
|`/status`|Returns `OK`|

## Project structure

```
MecLab-Project/
├── README.md
├── .gitignore
├── docs/                          (photos / wiring diagram go here)
└── firmware/
    └── traffic\_light\_esp8266/
        ├── traffic\_light\_esp8266.ino
        ├── secrets.h.example
        └── data/                  (web interface: HTML, CSS, JS, images, video)
```

## Project status

The ESP8266 hardware version was built and tested on the original board with three LEDs. The board is no longer available, so the hardware side is not currently being re-tested. The browser simulation is fully self-contained.

## Credits

* Web interface (HTML/CSS/JS) developed as part of a group project with: Tomoghna Mitra, Shalika Manoj.
* ESP8266 firmware and hardware integration: Hadassah Okogie.
* Car images were found through web image searches and their original sources are unknown.
* Background video: web source.


https://github.com/user-attachments/assets/9f284281-519e-4d57-9a28-e5779b132e3b


