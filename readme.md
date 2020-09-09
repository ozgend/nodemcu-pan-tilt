# pan/tilt with nodemcu esp12-e (esp8266) via websocket

a websocket pan/tilt controller for an esp8266 with two servos.

can be used for simple axis control such as camera tripod mount or laser turret.

## requirements
- either get a pan/tilt module or make one with two servo motors
- esp8266 module

## running
- [`definitions.h`] update following lines with your wireless network properties

  ```cpp
  const char *WIFI_SSID = "ssid";
  const char *WIFI_PASS = "pass";
  ```

  - [`definitions.h`] set ota credentials for esp module

  ```cpp
  const char *OTA_NAME = "ESP8266"; 
  const char *OTA_PASS = "otapass";
  ```

- [`definitions.h`] map and connect the io pins for servo motors
  ```cpp
  #define PIN_SERVO_X D2
  #define PIN_SERVO_Y D3
  ```

- [`definitions.h`] update the default, min, max rotation angles for servos
  ```cpp
  #define DEFAULT_X 90
  #define DEFAULT_Y 90
  #define MIN_X 0
  #define MAX_X 180
  #define MIN_Y 0
  #define MAX_Y 180
  ```

- set board to `esp8266` compatible module
- set baud rate to `115200`
- upload & go

## operating
- find host ip of the esp8266 module 
- go to `http://host_ip` (or mdns: http://esp8266.local)
- perform pan/tilt via adjusting sliders or keyboard arrow keys
