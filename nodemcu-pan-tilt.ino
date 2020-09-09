#include <FS.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Servo.h>
#include "definitions.h"

int axisX = DEFAULT_X;
int axisY = DEFAULT_Y;

ESP8266WebServer server(80);
WebSocketsServer ws(81);
File fsUploadFile;
Servo servoX;
Servo servoY;

String getContentType(String filename)
{
  if (filename.endsWith(".html"))
  {
    return "text/html";
  }
  else if (filename.endsWith(".css"))
  {
    return "text/css";
  }
  else if (filename.endsWith(".js"))
  {
    return "application/javascript";
  }
  else if (filename.endsWith(".ico"))
  {
    return "image/x-icon";
  }
  else if (filename.endsWith(".gz"))
  {
    return "application/x-gzip";
  }
  return "text/plain";
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

////
//// http handlers

void handle_404()
{
  if (!handle_readFile(server.uri()))
  {
    server.send(404, "text/plain", "404: File Not Found");
  }
}

bool handle_readFile(String path)
{
  Serial.println("handle_readFile: " + path);

  if (path.endsWith("/"))
  {
    path += "index.html";
  }

  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";

  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
  {
    if (SPIFFS.exists(pathWithGz))
    {
      path += ".gz";
    }

    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void handle_uploadFile()
{
  HTTPUpload &upload = server.upload();
  String path;

  if (upload.status == UPLOAD_FILE_START)
  {
    path = upload.filename;
    if (!path.startsWith("/"))
    {
      path = "/" + path;
    }
    if (!path.endsWith(".gz"))
    {
      String pathWithGz = path + ".gz";
      if (SPIFFS.exists(pathWithGz))
      {
        SPIFFS.remove(pathWithGz);
      }
    }

    Serial.print("handle_uploadFile name: ");
    Serial.println(path);
    fsUploadFile = SPIFFS.open(path, "w");
    path = String();
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    if (fsUploadFile)
    {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (fsUploadFile)
    {
      fsUploadFile.close();
      server.sendHeader("Location", "/upload_success.html"); // Redirect the client to the success page
      server.send(303);
    }
    else
    {
      server.send(500, "text/plain", "upload error");
    }
  }
}

void handle_wsEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("ws: [%u] disconnected!\n", num);
    break;

  case WStype_CONNECTED:
  {
    IPAddress ip = ws.remoteIP(num);
    Serial.printf("ws: [%u] connected [%d.%d.%d.%d] to [%s]\n", num, ip[0], ip[1], ip[2], ip[3], payload);
  }
  break;

  case WStype_TEXT:
    Serial.printf("ws: [%u] event [%s]\n", num, payload);
    handle_actionEvent(payload);
    break;
  }
}

void handle_actionEvent(uint8_t *payload)
{
  String data = (char *)payload;

  if (data[0] == 's') // set angle
  {
    axisX = data.substring(1, data.indexOf(',')).toInt();
    axisY = data.substring(data.indexOf(',') + 1).toInt();
    writeAxis(axisX, axisY);
  }
}

////
//// init & setup

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }

  Serial.println("\n");
  Serial.println("connected");
  Serial.print("ip: ");
  Serial.print(WiFi.localIP());
  Serial.println("\n");

  Serial.println("done - init WiFi");
}

void initMDNS()
{
  MDNS.begin(MDNS_NAME);

  Serial.print("done - init MDNS");
}

void initServer()
{
  server.on(
      "/upload.html", HTTP_POST, []() {
        server.send(200, "text/plain", "");
      },
      handle_uploadFile);

  server.onNotFound(handle_404);
  server.begin();

  Serial.println("done - init Server");
}

void initWS()
{
  ws.begin();
  ws.onEvent(handle_wsEvent);

  Serial.println("done - init WS");
}

void initServo()
{
  servoX.attach(PIN_SERVO_X);
  servoY.attach(PIN_SERVO_Y);
  //writeAxis(DEFAULT_X, DEFAULT_Y);

  Serial.println("done - init Servo");
}

void initOTA()
{
  ArduinoOTA.setHostname(OTA_NAME);
  ArduinoOTA.setPassword(OTA_PASS);

  ArduinoOTA.onStart([]() {
    Serial.println("OTA start");
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("");
    Serial.println("OTA end");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA progress: %u%%\n", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("OTA_AUTH_ERROR");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("OTA_BEGIN_ERROR");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("OTA_CONNECT_ERROR");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("OTA_RECEIVE_ERROR");
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println("OTA_END_ERROR");
    }
    else
    {
      Serial.printf("OTA error: [%u]: ", error);
    }
  });

  ArduinoOTA.begin();

  Serial.println("done - init OTA");
}

void initSPIFFS()
{
  SPIFFS.begin();

  Dir dir = SPIFFS.openDir("/");
  while (dir.next())
  {
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    Serial.printf("\t %s (%zu bytes)\n", fileName.c_str(), fileSize);
  }

  Serial.println("done - init SPIFFS");
}

void setup()
{
  Serial.begin(SERIAL_BAUD);
  Serial.println("init");

  initWiFi();
  initMDNS();
  initOTA();
  initSPIFFS();
  initServer();
  initWS();
  initServo();

  Serial.println("done - setup");
}

void loop()
{
  ws.loop();
  server.handleClient();
  ArduinoOTA.handle();
}
