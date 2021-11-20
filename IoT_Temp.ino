
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

#define ONE_WIRE_BUS 2  // NodeMCU GPIO pin number where DS18b20 data pin connected

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
ESP8266WebServer server(80);

//wifi settings
const char* ssid = "Embedded-Programmer"; //Use your own SSID
const char* password =  "iotproject123";  //Use your own Passoword

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  sensors.begin();

  //Wi-Fi 
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting ...");
  }

  Serial.print("Connected to WiFi, IP Address is: ");
  Serial.println(WiFi.localIP()); 
  server.on("/", Connect);
  server.onNotFound(Err_Connect);
  server.begin();
  
}


int room_temp;

void loop(void)
{ 
  sensors.requestTemperatures();
  room_temp = sensors.getTempCByIndex(0);
  
  if(room_temp != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Room Temperature is: ");
    Serial.println(room_temp);
  } 
  
  else
  {
    Serial.println("Error: Could not read temperature !");
  }
   server.handleClient();
}

void Connect() {

  server.send(200, "text/html", HTML_Code(room_temp)); 
}

void Err_Connect(){
  server.send(404, "text/plain", " Page Not found");
}

String HTML_Code(int Temp){
  
  String message = "<!DOCTYPE html> <html>\n";
  
  message += "<head> <meta name = \"viewport\" content = \" initial-scale = 1.0, width = device-width  \">\n";
  message += "<meta http-equiv = \"refresh\" content=\"3\">"; 
  message += "<title>EP </title>\n";
  message += "<style>html { font-family: Arial; display: inline-block; margin: 5px auto; text-align: center;}\n";
  message += "body{margin-top: 80px;} h1 {color: #009000;margin: 40px auto 10px;}\n";
  message += "p {font-size: 50px;color: #F00000; margin-bottom: 15px;}\n";
  
  message += "</style>\n";
  message += "</head>\n";
  message += "<body>\n";
  message += "<div id=\"webpage\">\n";
  message += "<h1>IoT  Temperature  Monitoring</h1>\n";
  
  message += "<p>Room Temp: ";
  message += Temp;
  message += " &#8451</p>";
  
  message += "</div>\n";
  message += "</body>\n";
  message += "</html>\n";
  return message;
}
