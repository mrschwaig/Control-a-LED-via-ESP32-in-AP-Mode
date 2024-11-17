#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define LED 32
#define ssid "ESP_LED_TEST"
#define password "123456789"

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

bool LEDStatus = LOW;

String webCode();
void ConnectionOn();
void switchLedOn();
void switchLedOff();
void urlNotFound();

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  
  server.on("/", ConnectionOn);
  server.on("/ledon", switchLedOn);
  server.on("/ledoff", switchLedOff);
  server.onNotFound(urlNotFound);

  server.begin();

  Serial.println("WiFi Server gestartet");
}

void loop()
{
  server.handleClient();

  if(LEDStatus) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}

String webCode() {
  String htmlcode = "<!DOCTYPE html> <html>\n";
  htmlcode +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  htmlcode +="<title>LED Control</title>\n";
  htmlcode +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  htmlcode +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  htmlcode +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  htmlcode +=".button-on {background-color: #3498db;}\n";
  htmlcode +=".button-on:active {background-color: #2980b9;}\n";
  htmlcode +=".button-off {background-color: #34495e;}\n";
  htmlcode +=".button-off:active {background-color: #2c3e50;}\n";
  htmlcode +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  htmlcode +="</style>\n";
  htmlcode +="</head>\n";
  htmlcode +="<body>\n";
  htmlcode +="<h1>ESP32 LED Steuerung</h1>\n";
  htmlcode +="<h3>Du kannst hier eine LED ein und aus schalten.\n";

  if(LEDStatus)
  {htmlcode +="<p>LED Status: AN</p><a class=\"button button-off\" href=\"/ledoff\">AUS</a>\n";}
  else
  {htmlcode +="<p>LED Status: AUS</p><a class=\"button button-on\" href=\"/ledon\">AN</a>\n";}

  htmlcode +="</body>\n";
  htmlcode +="</html>\n";
  return htmlcode;
}

void ConnectionOn() {
  LEDStatus = LOW;

  Serial.println("LED Status: AUS");
  server.send(200, "text/html", webCode());
}

void switchLedOn() {
  LEDStatus = HIGH;

  Serial.println("LED Status: AN");
  server.send(200, "text/html", webCode());
}

void switchLedOff() {
  LEDStatus = LOW;

  Serial.println("LED Status: AUS");
  server.send(200, "text/html", webCode());
}

void urlNotFound(){
  server.send(404, "text/plain", "Not found");
}