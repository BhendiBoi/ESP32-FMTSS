/*
ESP32 CAM Main System Code
Unit of ESP32 Face Mask + Temperature + Sanitisation System (ESP32 FMTSS)
Documented on 12th March 2021 
Code Written By Sanath Shet
Special Thanks to Nandan Bhandary and Rishon Fernandes
Pre-Use Checklist (VERY IMPORTANT):-
1. Comment out or Edit the "Wire.begin()" statement in Adafruit_MLX90614.cpp Line 32
2. Turn all wire->function() statements into Wire.function() statements in Adafruit_SSD1306.cpp
3. Comment out or Edit the "Wire.begin()" statement in Adafruit_SSD1306.cpp Line 490
4. Follow Exact Pinout (as given below) Or Modify Integer Values
5. Make Sure SD Card is atttached to DFPlayer and File Order Matches the one given below
Pinout :-
OLED RES(reset) -> GPIO14 (If RES pin is not present, modify "#define OLED_RESET 14" to "#define OLED_RESET -1" main.cpp line 67)
OLED D1/SDA -> GPIO13 (Use Pull-Up resistors if necessary)
OLED D0/SCL -> GPIO15 (Use Pull-Up resistors if necessary)
OLED DC -> GND (ignore if pins not present on OLED)
OLED CS -> GND (ignore if pins not present on OLED)
MLX90614 SDA -> GPIO13 (3.3V ONLY)
MLX90614 SCL -> GPIO15 (3.3V ONLY)
DFPlayer Mini RX -> GPIO2 (Through a 1K Resistor)
DFPlayer SPK1 -> Speaker 
DFPlayer SPK2 -> Speaker
FTDI RX -> GPIO1
FTDI TX -> GPIO3
DF Player File Order:-
1) FaceTheCamera.mp3
2) WearMask.mp3
3) CheckTemp.mp3
4) TempCritical.mp3
5) Sanitize.mp3
6) Proceed.mp3
OLED graphics variable names:-
1) facethecamera:- tells the user to face the OV2640
2) wearmask:- tells the user to wear a mask
3) 
*/

#include <Arduino.h>           //necessary for .cpp extension
#include <Adafruit_MLX90614.h> //For Working of MLX90614 Contactless Temperature Sensor
#include <Adafruit_GFX.h>      //Dependency of Adafruit_SSD1306
#include <Adafruit_SSD1306.h>  //For Working of OLED display
#include <Wire.h>              //I2C Library
#include <DFFunctions.h>       //contains code for initialization and working of DFPlayer Mini
#include <WebServer.h>         //For Working of WebServer
#include <esp32cam.h>          //For Working of ESP32 CAM
#include <OLEDGraphics.h>      //Contains bitmap images that OLED can display
#include "soc/soc.h"           //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems

//Replace with your network credentials
const char *ssid = "JioFiber -  2.4";
const char *password = "Jio@1102";

IPAddress local_IP(192, 168, 29, 199); // Set your Static IP address
IPAddress gateway(192, 168, 1, 1);     // Set your Gateway IP address
IPAddress subnet(255, 255, 0, 0);

int TXD = 2; //Pin to communicate with DFPlayer

int IOport = 81;  //Port for /mask, /nomask server
int camport = 80; //Port for camera

bool mask_detected = false; //Boolean variable to record if mask was worn by user
bool temp_normal = false; //Boolean variable to record if temperature of user was normal
bool sanitized = false; //Boolean variable to record if user sanitized his hands

String header; 

unsigned long currentTime; 
unsigned long previousTime;
const long timeoutTime = 2000;

static auto loRes = esp32cam::Resolution::find(320, 240);
static auto hiRes = esp32cam::Resolution::find(800, 600);

int sig = 12;
unsigned long timestart;
unsigned long timepassed;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 14
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DFF DF(TXD); 

WebServer camserver(camport);
WiFiServer IOserver(IOport);

Adafruit_MLX90614 MLX = Adafruit_MLX90614();

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
float verifyTemp()
{
  float rectemp[10];
  float sumtemp = 0;
  for (int i = 0; i < 10; i++)
  {
    rectemp[i] = MLX.readObjectTempC() + 3;
    Serial.println(rectemp[i]);
    delay(200);
    sumtemp = sumtemp + rectemp[i];
    Serial.println("");
    Serial.println(sumtemp);
  }
  float avgtemp = sumtemp / 10;
  return avgtemp;
}
void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200); //establish serial communication
  Serial.setDebugOutput(false); 
  wifiConnect(); //connect to WiFi
  startCameraServer(); //start a camera streaming server
  IOserver.begin(); //start a server to communicate with Face-Mask-Detection Algorithm
  Wire.begin(13, 15); //establish I2C connection between OLED and MLX90614 
  MLX.begin();                               
  DF.begin();
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);     
  display.display();                                        
  display.clearDisplay();
  display.drawBitmap(5, 12, facethecamera, 118, 40, WHITE); //Tell user to face the OV2640 camera with graphics
  display.display();
  DF.play(1); //Play face the camera audio file
}
void loop()
{
  float temp; //variable to record temperature of user

  while (mask_detected == false) 
  {
    camserver.handleClient(); 
    byte output = IOListen();
    if (output == 1)
    {
      mask_detected = true;
      display.clearDisplay();
      display.drawBitmap(0, 12, checktemp, 128, 40, WHITE);
      display.display();
      DF.play(3);
      Serial.println("Mask Detected");
      output = 3;
    }
    else if (output == 0)
    {
      display.clearDisplay();
      display.drawBitmap(0, 12, wearmask, 128, 40, WHITE);
      display.display();
      DF.play(2);
      Serial.println("Mask Not Detected");
      output = 3;
    }
  }
  while (temp_normal == false)
  {
    temp = MLX.readObjectTempC() + 3;
    delay(1000);
    Serial.println(temp);
    if (temp <= 35)
    {
      Serial.println("Wrist not Detected");
    }
    else
    {
      Serial.println("Wrist Detected");
      float avgtemp = verifyTemp();
      Serial.print("avgtemp = ");
      Serial.println(avgtemp);

      if (avgtemp >= 38 || avgtemp <= 35)
      {
        display.clearDisplay();
        display.drawBitmap(0, 12, tempcritical, 128, 40, WHITE);
        display.display();
        DF.play(4);
        Serial.println("too high!");
        avgtemp = 0;
      }
      else
      {
        temp_normal = true;
        display.clearDisplay();
        display.drawBitmap(44, 12, sanitize, 40, 40, WHITE);
        display.display();
        DF.play(5);
        Serial.println("normal");
      }
    }
  }
  while (sanitized == false)
  {
    int sigstate = digitalRead(sig);
    Serial.println(sigstate);
    timestart = millis();
    while (sigstate == 0)
    {
      sigstate = digitalRead(sig);
      Serial.println(sigstate);
      timepassed = millis() - timestart;
      Serial.println(timepassed);
      if (timepassed > 1000)
      {
        Serial.println("turning on LED");
        timepassed = 0;
        pinMode(sig, OUTPUT);
        digitalWrite(sig, HIGH);
        delay(1000);
        pinMode(sig, INPUT_PULLUP);
        while (sigstate == 0)
        {
          sigstate = digitalRead(sig);
        }
      }
    }
    delay(500);
  }
}