#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

// Wi-Fi credentials live in secrets.h (copy secrets.h.example and edit it).
#include "secrets.h"

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int red = 0;
const int green = 2;
const int yellow = 4;
bool redstate=false;
bool greenstate=false;
bool yellowstate=false;
bool isYellowBlinking = false;
unsigned long lastBlinkTime = 0;
bool yellowBlinkState = false;



void handleLED(String color, bool state){
  int pin;
  if(color == "red"){
    pin=red;
    redstate=state;
    digitalWrite(red,state ? LOW:HIGH);
    }
  else if(color == "green"){
    pin=green;
    greenstate=state;
    digitalWrite(green,state ? LOW:HIGH);
  }
  else if(color == "yellow"){
    pin=yellow;
    yellowstate=state;
    digitalWrite(yellow,state ? LOW:HIGH);
  }
  else{
    server.send(404, "text/plain", "Invalid LED");
    return;
   }
   server.send(200, "text/plain", "OK");
}

void handleLEDStatus(){
  server.send(200, "text/plain", "OK");
}


void handleRoot() {
  digitalWrite(red, 1);
  digitalWrite(red, 0);
  digitalWrite(green, 1);
  digitalWrite(green, 0);
  digitalWrite(yellow, 1);
  digitalWrite(yellow, 0);
  loadFromSPIFFS("/index.html");
  
}

void handleNotFound() {
  digitalWrite(red, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
  server.send(404, "text/plain", message);
  digitalWrite(red, 0);
}
bool loadFromSPIFFS(String path) {
  String dataType = "text/plain";
  if (path.endsWith(".html")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  else if (path.endsWith(".png")) dataType = "image/png";
  else if (path.endsWith(".jpg") || path.endsWith(".jpeg")) dataType = "image/jpeg";
  else if (path.endsWith(".gif")) dataType = "image/gif";
  else if (path.endsWith(".mp4")) dataType = "video/mp4";
 
  Serial.print("Requested page -> ");
  Serial.println(path);
  if (SPIFFS.exists(path)){
      File dataFile = SPIFFS.open(path, "r");
      if (!dataFile) {
          handleNotFound();
          return false;
      }
 
      if (server.streamFile(dataFile, dataType) != dataFile.size()) {
        Serial.println("Sent less data than expected!");
      }else{
          Serial.println("Page served!");
      }
 
      dataFile.close();
  }else{
      handleNotFound();
      return false;
  }
  return true;
}
 
 
void setup(void) {
  SPIFFS.begin();
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(yellow, HIGH);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print(F("Inizializing FS..."));
  if (SPIFFS.begin()){
    Serial.println(F("done."));
  }else{
    Serial.println(F("fail."));
  }
 

  if (MDNS.begin("esp8266")) { Serial.println("MDNS responder started"); }

  server.on("/", handleRoot);
  server.on("/carr1.png",     []() { loadFromSPIFFS("/carr1.png");}); 
  server.on("/carr3.png",     []() { loadFromSPIFFS("/carr3.png");}); 
  server.on("/carr6.png",     []() { loadFromSPIFFS("/carr6.png");}); 
  server.on("/carr7.png",     []() { loadFromSPIFFS("/carr7.png");});
  server.on("/carr9.png",     []() { loadFromSPIFFS("/carr9.png");}); 
  server.on("/index.html",     []() { loadFromSPIFFS("/index.html");}); 
  server.on("/script.js",     []() { loadFromSPIFFS("/script.js");}); 
  server.on("/style.css",     []() { loadFromSPIFFS("/style.css");});
  server.on("/vis.mp4",      []() { loadFromSPIFFS("/vis.mp4");});
  server.on("/redon",     []() { handleLED("red", true); });
  server.on("/redoff",    []() { handleLED("red", false); });
  server.on("/greenon",   []() { handleLED("green", true); });
  server.on("/greenoff",  []() { handleLED("green", false); });
  server.on("/yellowon",  []() { handleLED("yellow", true); });
  server.on("/yellowoff", []() {
  isYellowBlinking = false;
  yellowBlinkState = false;
  digitalWrite(yellow, HIGH);  
  yellowstate = false;
  server.send(200, "text/plain", "Yellow OFF");
});
  server.on("/yellowblink", []() {
  isYellowBlinking = true;
  yellowBlinkState = true;
  lastBlinkTime = millis();
  Serial.println("Yellow blink started");
  server.send(200, "text/plain", "Yellow blinking started");
});


  server.on("/status", handleLEDStatus);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/gif", []() {
    static const uint8_t gif[] PROGMEM = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    // Set the background to a random set of colors
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  

  if (isYellowBlinking) {
    if (millis() - lastBlinkTime >= 500) {  // toggle every 500ms
      lastBlinkTime = millis();
      yellowBlinkState = !yellowBlinkState;
      digitalWrite(yellow, yellowBlinkState ? LOW : HIGH); // LOW = ON, HIGH = OFF (check your logic)
    }
  }
}
