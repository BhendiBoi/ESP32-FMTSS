#include <Arduino.h>
#include <Adafruit_MLX90614.h> //Remember to Modify!! (Comment "Wire.begin()" in Adafruit_SSD1306.CPP)
#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <DFFunctions.h>
#include <WebServer.h>
#include <esp32cam.h>
#include "soc/soc.h"          //disable brownout problems
#include "soc/rtc_cntl_reg.h" //disable brownout problems

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

Adafruit_MLX90614 MLX = Adafruit_MLX90614();

bool mask_detected = false;
bool temp_normal = false;
bool sanitized = false;
String header;
// Current time
unsigned long currentTime;
// Previous time
unsigned long previousTime;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

int IOport = 81;
int camport = 80;

WebServer camserver(camport);
WiFiServer IOserver(IOport);

static auto loRes = esp32cam::Resolution::find(320, 240);
static auto hiRes = esp32cam::Resolution::find(800, 600);

void wifiConnect()
{
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected with IP address: http://");
  Serial.println(local_IP);
}

void handleJpgHi()
{
  if (!esp32cam::Camera.changeResolution(hiRes))
  {
    Serial.println("SET-HI-RES FAIL");
  }
  auto frame = esp32cam::capture();
  if (frame == nullptr)
  {
    Serial.println("CAPTURE FAIL");
    camserver.send(503, "", "");
    return;
  }
  Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));

  camserver.setContentLength(frame->size());
  camserver.send(200, "image/jpeg");
  WiFiClient client = camserver.client();
  frame->writeTo(client);
}

void startCameraServer()
{
  camserver.begin();
  Serial.println();

  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);

    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }

  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("  /cam-hi.jpg");

  camserver.on("/cam-hi.jpg", handleJpgHi);

  camserver.begin();
}

byte IOListen()

{
  // put your main code here, to run repeatedly:
  WiFiClient client = IOserver.available(); // Listen for incoming clients

  if (client)
  { // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    //Serial.println("New Client."); // print a message out in the serial port
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { // loop while the client's connected
      currentTime = millis();
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        //Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /nomask") >= 0)
            {
              Serial.println("Mask Not Detected");
              return 0;
            }
            else if (header.indexOf("GET /mask") >= 0)
            {
              Serial.println("Mask Detected");
              return 1;
            }
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
  }
}

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  wifiConnect();
  startCameraServer();
  IOserver.begin();
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
    camserver.handleClient();
    byte output = IOListen();
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
      byte output = IOListen();
    }
  }
}
