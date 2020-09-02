#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include "definitions.h"

int axisX = DEFAULT_X;
int axisY = DEFAULT_Y;

ESP8266WebServer server(80);

Servo servoX;
Servo servoY;

void handle_root() {
  server.send_P(200, "text/html", HTML_root);
}

void handle_moveLeft()
{
  writeAxis(axisX-1, axisY);
  server.send(200, "text/plain", "x: " + String(axisX) + "  ,  y: " + String(axisY));
}

void handle_moveRight()
{
  writeAxis(axisX+1, axisY);
  server.send(200, "text/plain", "x: " + String(axisX) + "  ,  y: " + String(axisY));
}

void handle_moveUp()
{
  writeAxis(axisX, axisY+1);
  server.send(200, "text/plain", "x: " + String(axisX) + "  ,  y: " + String(axisY));
}

void handle_moveDown()
{
  writeAxis(axisX, axisY-1);
  server.send(200, "text/plain", "x: " + String(axisX) + "  ,  y: " + String(axisY));
}

void handle_axisReset()
{
  writeAxis(DEFAULT_X, DEFAULT_Y);
  server.send(200, "text/plain", "x: " + String(axisX) + "  ,  y: " + String(axisY));
}

void writeAxis(int x, int y)
{
  if (x > MIN_X && x < MAX_X)
  {
    servoX.write(x);
    axisX = x;
  }

  if (y > MIN_Y && y < MAX_Y)
  {
    servoY.write(y);
    axisY = y;
  }
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  Serial.println("init");

  servoX.attach(PIN_SERVO_X);
  servoY.attach(PIN_SERVO_Y);

  //writeAxis(DEFAULT_X, DEFAULT_Y);

  Serial.println("connecting");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("connected");

  server.on("/", handle_root);
  server.on("/move/left", handle_moveLeft);
  server.on("/move/right", handle_moveRight);
  server.on("/move/up", handle_moveUp);
  server.on("/move/down", handle_moveDown);
  server.on("/axis/reset", handle_axisReset);

  server.begin();

  Serial.println("server started");
  Serial.println(WiFi.localIP());
}

void loop()
{
  server.handleClient();
}
