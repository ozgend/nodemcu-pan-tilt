# pan/tilt with nodemcu esp12-e (esp8266)

a pan/tilt controller for an esp8266 with two servos

can be used for simple axis control such as camera tripod mount or laser turret.

## requirements
- either get a pan/tilt module or make one with two servo motors
- esp8266 module

## running
- [`definitions.h`] update following lines with your wireless network properties

  ```cpp
  char *WIFI_SSID = "ssid";
  char *WIFI_PASS = "pass";
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

- set board to an `esp8266` compatible module
- baud rate `115200`
- upload & go

## operating
- find host ip of the esp8266 module 
- go to `http://host_ip`
- perform pan/tilt via clicking buttons or pressing keyboard arrow keys