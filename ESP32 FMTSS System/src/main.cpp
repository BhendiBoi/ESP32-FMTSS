#include <Arduino.h>
#include <Adafruit_MLX90614.h> //Remember to Modify!! (Comment "Wire.begin()" in Adafruit_SSD1306.CPP)
#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <WebServerFunctions.h> //Library containing all necessary camera pins and server initialization functions
#include <DFFunctions.h>

int streamPort = 80; //-> Port number for StreamWebServer
int IOPort = 81;     //-> Port number for IOWebServer

WSF streamserver(streamPort);
WSF IOserver(IOPort);

int RXD;
int TXD;

DFF DF(RX, TX);

/*DF Player File Order:-
1) FaceTheCamera.mp3
2) WearMask.mp3
3) CheckTemp.mp3
4) TempCritical.mp3
5) Sanitize.mp3
6) Proceed.mp3
*/

//Replace with your network credentials
const char *ssid = "JioFiber -  2.4"; //uncomment (made to remind replacement of SSID and Password)
const char *password = "Jio@1102";

// Set your Static IP address
IPAddress local_IP(192, 168, 29, 199);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);

WiFiServer server(IOPort);

Adafruit_MLX90614 MLX = Adafruit_MLX90614();

bool mask_detected = false;
bool temp_normal = false;
bool sanitized = false;

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  wifiConnect(ssid, password, local_IP, gateway, subnet);
  streamserver.startCameraServer();
  IOserver.startIOServer(server);
  Wire.begin(13, 15);
  MLX.begin();
  DF.begin();
  //display.begin(SSD1306_SWITCHCAPVCC);
  // display.display();
  // display.clearDisplay();
  DF.play(1);
}
void loop()
{
  while (mask_detected == false)
  {
    if (IOserver.IOListen() == true)
    {
      mask_detected == true;
      // display.cleardisplay();
      // display.setCursor(x, y)
      // display.drawBitmap(checkTemp);
      DF.play(3);
    }
    else
    {
      // display.cleardisplay();
      // display.setCursor(x, y)
      // display.drawBitmap(wearMask);
      DF.play(2);
    }
  }
  while (temp_normal == false)
  {
  }
}
