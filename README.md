# nemsensor
Basic automated irrigation with Arduino; using lcd shield with buttons for GUI, rtc module for 24hr based time-window irrigation, soil moisture sensor, and relay module to power a water pump.

### Parts
* Arduino Uno
* LCD Shield with Keypad
* DS1302 Time Module
* Relay Module
* Moisture Sensor
* Breadboard (optional)
![Prototyping](https://github.com/bariscc/nemsensor/blob/master/nemsensor_bb.png)

### Connections
Pin list
```
sensor = A5;
relay = 3;
rtc enable (rst) = 11;
rtc io (data) = 12;
rtc clock (clk) = 13;
```
`4-10` used by LCD, `A0` used by keypad.

### Installation
After connecting pins, upload the `setup/setup.ino` to Uno to set current time and default configuration to device. You should see "SETUP COMPLETE" on success. Then, upload `nemsensor/nemsensor.ino`.

That's all!

### GUI
Menu
  * Status (date-time, soil moisture value, operation status)
  * Settings    
    * Target Moisture (minimum soil moisture value to meet)
    * Irrigation Start Hour (prevents relay activation before this)
    * Irrigation End Hour (prevents relay activation after this)
    * Sensor Interval (soil moisture check interval)
  
Features
  * Lcd light timeout (after 30 sec. inactivity)

Status
  * `---` means time is right and relay is open
  * `-/-` means time is right and relay is closed
  * `zZz` means time is not right, therefore relay is closed
  
### Controls
`select` button = next menu

On Status
* `down` button = save brightness
* `right` button = increase brightness
* `down` button = decrease brightness
* `up` button = read sensor manually (restarts soil moisture check interval)

On Settings
* `down` button = save value
* `right` button = increase value
* `down` button = decrease value
