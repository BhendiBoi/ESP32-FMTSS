#include <Arduino.h>
// #include <Adafruit_MLX90614.h> //Remember to Modify!! (Comment "Wire.begin()" in Adafruit_SSD1306.CPP)
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <WebServerFunctions.h> //Library containing all necessary camera pins and server initialization functions

int streamPort = 80; //Port number for StreamWebServer
int IOPort = 81; //Port number for IOWebServer

WSF streamserver(streamPort); 
WSF IOserver(IOPort); 

//Replace with your network credentials
const char *ssid = "JioFiber -  2.4";  //uncomment (made to remind replacement of SSID and Password)
const char *password = "Jio@1102";

// Set your Static IP address
IPAddress local_IP(192, 168, 29, 199);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);

WiFiServer server(IOPort);

//Adafruit_MLX90614 MLX = Adafruit_MLX90614();

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  wifiConnect(ssid, password, local_IP, gateway, subnet);
  streamserver.startCameraServer();
  IOserver.startIOServer(server);
  // Wire.begin(13, 15);
  // MLX.begin();
  // display.begin(SSD1306_SWITCHCAPVCC);
  // display.display();
  // display.clearDisplay();
}
void loop()
{
  IOserver.IOListen();
}
