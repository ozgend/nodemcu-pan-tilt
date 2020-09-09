#ifndef definitions_h
#define definitions_h

const char *WIFI_SSID = "ssid";
const char *WIFI_PASS = "pass";

const char *OTA_NAME = "ESP8266"; 
const char *OTA_PASS = "otapass";

const char *MDNS_NAME = "esp8266"; 

#define SERIAL_BAUD 115200

#define PIN_SERVO_X D2
#define PIN_SERVO_Y D3

#define DEFAULT_X 90
#define DEFAULT_Y 90
#define MIN_X 0
#define MAX_X 180
#define MIN_Y 0
#define MAX_Y 180

#endif
