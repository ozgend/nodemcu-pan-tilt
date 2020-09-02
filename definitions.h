#ifndef definitions_h
#define definitions_h

#define SERIAL_BAUD 115200

char *WIFI_SSID = "ssid";
char *WIFI_PASS = "pass";

#define PIN_SERVO_X D2
#define PIN_SERVO_Y D3

#define DEFAULT_X 90
#define DEFAULT_Y 90
#define MIN_X 0
#define MAX_X 180
#define MIN_Y 0
#define MAX_Y 180

const char HTML_root[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
  <style>
    *,
    .iframe * {
      font-family: Arial, Helvetica, sans-serif;
      text-decoration: none;
      color: lightgreen;
      background-color: #111111;
      border: none;
    }

    a {
      display: block;
      padding: 10px;
      margin: 10px;
      border: 1px solid #333333;
    }

    iframe {
      overflow: hidden;
      color: white;
      width: 180px;
      height: 90px;
    }
  </style>
  <script>
    const listener = () => {
      document.addEventListener('keydown', (e) => {
        document.querySelector(`[key-bind='${e.code}']`).click();
      });
      console.log('listener registered');
    }
    const responder = () => {
      document.querySelector('[name=frm]').contentDocument.head.insertAdjacentHTML('beforeend', `<style type="text/css">*{color:lightgreen;}</style>`);
    }
    window.onload = () => listener();
  </script>
</head>

<body>
  <center>
    <h1>camera axis</h1><br>
    <table>
      <tr>
        <td> </td>
        <td> <a href="/move/up" target="frm" key-bind="ArrowUp">▲</a> </td>
        <td> </td>
      </tr>
      <tr>
        <td> <a href="/move/left" target="frm" key-bind="ArrowLeft">◄</a> </td>
        <td> <a href="/axis/reset" target="frm" key-bind="KeyR">↻</a> </td>
        <td> <a href="/move/right" target="frm" key-bind="ArrowRight">►</a> </td>
      </tr>
      <tr>
        <td> </td>
        <td> <a href="/move/down" target="frm" key-bind="ArrowDown">▼</a> </td>
        <td> </td>
      </tr>
    </table>
    <iframe name="frm" scrolling="no" onload="responder(this)"><br>
  </center>
</body>

</html>
)=====";

#endif
