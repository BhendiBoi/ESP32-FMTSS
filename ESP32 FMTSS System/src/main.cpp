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

int RXD = 13;
int TXD = 15;

DFF DF(RXD, TXD);

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

void verifyTemp()
{
  int rectemp[10];
  int sumtemp;
  int criticalcount;
  for (int i = 0; i < 10; i++;)
  {
    rectemp[i] = MLX.readObjectTempC() + 3;
    delay(100);
    sumtemp = sumtemp + rectemp[i];
  }
  int avgtemp = sumtemp / 10;
}

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
  int temp;
  while (mask_detected == false)
  {
    checkWifiConnection(ssid, password);
    byte output = IOserver.IOListen();
    if (output == 1)
    {
      //mask_detected = true;
      // display.cleardisplay();
      // display.setCursor(x, y)
      // display.drawBitmap(checkTemp);
      DF.play(2);
      // Serial.println("Mask Detected");
      output = 3;
    }
    else if (output == 0)
    {
      // display.cleardisplay();
      // display.setCursor(x, y)
      // display.drawBitmap(wearMask);
      DF.play(3);
      // Serial.println("Mask Not Detected");
      output = 3;
    }
    else
    {
      byte output = IOserver.IOListen();
    }
  }
  while (temp_normal == false)
  {
    temp = MLX.readObjectTempC() + 3;
    delay(10);
    if (temp <= 35)
    {
      // display.drawBitmap(checktemp);
    }
    else
    {
      verifyTemp();

      if (avgtemp >= 38 || avgtemp <= 35)
      {
        //display.drawBitmap(checkTemp);
        criticalcount++;
      }
    }
  }
}
