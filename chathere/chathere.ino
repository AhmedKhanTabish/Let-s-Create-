#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

const char *ssid = "ChatServer";
const char *password = "12345678";

// Declare the webpage content before setup
const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP8266 Chat</title>
  <script>
    var socket = new WebSocket("ws://" + window.location.hostname + ":81/");
    function sendMessage() {
      var msg = document.getElementById("msg").value;
      socket.send(msg);
      document.getElementById("chatbox").innerHTML += "<b>You:</b> " + msg + "<br>";
      document.getElementById("msg").value = "";
    }
    socket.onmessage = function(event) {
      document.getElementById("chatbox").innerHTML += "<b>Guest:</b> " + event.data + "<br>";
    };
  </script>
</head>
<body>
  <h2>ESP8266 Chat Room</h2>
  <div id="chatbox" style="height: 200px; overflow-y: scroll; border: 1px solid black; padding: 5px;"></div>
  <input type="text" id="msg" placeholder="Type message..." />
  <button onclick="sendMessage()">Send</button>
</body>
</html>
)rawliteral";

AsyncWebServer server(80);
WebSocketsServer webSocket(81);

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_TEXT:
      for (int i = 0; i < webSocket.connectedClients(); i++) {
        if (i != num) {
          webSocket.sendTXT(i, (char*)payload);
        }
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  Serial.println("Chat Server Running...");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", webpage); // Now it works since 'webpage' is declared before use
  });

  server.begin();
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  webSocket.loop();
}
